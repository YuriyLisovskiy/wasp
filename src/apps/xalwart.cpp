/**
 * apps/xalwart.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./xalwart.h"

// C++ libraries.
#include <iostream>

// Core libraries.
#include <xalwart.core/path.h>

// Framework libraries.
#include "../management/module.h"
#include "../core/parsers/url_parser.h"
#include "../core/parsers/query_parser.h"
#include "../core/parsers/multipart_parser.h"
#include "../urls/resolver.h"


__APPS_BEGIN__

MainApplication::MainApplication(
	conf::Settings* settings,
	std::function<std::shared_ptr<net::IServer>(
		core::ILogger*,
		collections::Dict<std::string, std::string>
	)> server_initializer
) : server_initializer(std::move(server_initializer))
{
	core::InterruptException::initialize();
	if (!settings)
	{
		throw core::ImproperlyConfigured(
			"Setting must be configured in order to use the application."
		);
	}

	this->settings = settings;
	this->settings->prepare();
	this->_perform_checks();

	this->_setup_commands();

	for (auto& command : this->_commands)
	{
		this->_help_message += command.second->usage() + '\n';
	}

	if (!this->_help_message.empty())
	{
		this->_help_message = "Available commands:\n\n" + this->_help_message;
	}
	else
	{
		this->_help_message = "Application has not commands.\n\n";
	}
}

void MainApplication::execute(int argc, char** argv)
{
	if (argc > 1)
	{
		try
		{
			if (this->_commands.find(argv[1]) != this->_commands.end())
			{
				this->_commands[argv[1]]->run_from_argv(argc, argv);
			}
			else
			{
				std::cout << "Command \"" << argv[1] << "\" is not found.\n\n";
			}
		}
		catch (const core::CommandError& exc)
		{
			this->settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const core::InterruptException& exc)
		{
			this->settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
	}
	else
	{
		std::cout << this->_help_message;
	}
}

void MainApplication::_setup_commands()
{
	// Retrieve commands from INSTALLED_MODULES and
	// check if modules' commands do not override
	// settings commands.
	for (auto& installed_module : this->settings->INSTALLED_MODULES)
	{
		this->_extend_settings_commands_or_error(
			installed_module->get_commands(),
			[installed_module](const std::string& cmd_name) -> std::string {
				return "Module with name '" + installed_module->get_name() +
				       "' overrides commands with '" + cmd_name + "' command";
			}
		);
	}

	auto default_commands = management::CoreManagementModuleConfig(
		this->settings, [this](
			core::ILogger* logger,
			collections::Dict<std::string, std::string> kwargs
		) -> std::shared_ptr<net::IServer>
		{
			auto server = this->server_initializer(logger, std::move(kwargs));
			server->setup_handler(this->make_handler());
			return server;
		}
	).get_commands();

	// Check if user-defined commands do not override
	// default commands, if not, append them to settings.COMMANDS
	this->_extend_settings_commands_or_error(
		default_commands,
		[](const std::string& cmd_name) -> std::string {
			return "Attempting to override '" + cmd_name + "' command which may produce an undefined behaviour.";
		}
	);
}

void MainApplication::_extend_settings_commands_or_error(
	const std::vector<std::shared_ptr<cmd::BaseCommand>>& from,
	const std::function<std::string(const std::string& cmd_name)>& err_fn
)
{
	for (auto& command : from)
	{
		if (std::find_if(
			this->_commands.begin(),
			this->_commands.end(),
			[command](const std::pair<
				std::string, std::shared_ptr<cmd::BaseCommand>
			>& pair) -> bool {
				return command->name() == pair.first;
			}
		) != this->_commands.end())
		{
			this->settings->LOGGER->warning(err_fn(command->name()));
		}

		this->_commands[command->name()] = command;
	}
}

void MainApplication::_perform_checks()
{
	std::cout << "Performing checks..." << std::endl;
	if (!this->settings->LOGGER)
	{
		throw core::ImproperlyConfigured("LOGGER instance must be configured");
	}

	size_t err_count = 0;
	if (!this->settings->TEMPLATE_ENGINE)
	{
		this->settings->LOGGER->error(
			"TEMPLATE_ENGINE must be configured in order to use the application."
		);
		err_count++;
	}

	if (this->settings->MIDDLEWARE.empty())
	{
		this->settings->LOGGER->warning("You have not added any middleware.");
	}

	if (this->settings->BASE_DIR.empty())
	{
		this->settings->LOGGER->error(
			"BASE_DIR must not be empty in order to use the application."
		);
		err_count++;
	}
	else if (!path::exists(this->settings->BASE_DIR))
	{
		this->settings->LOGGER->error(
			"BASE_DIR must exist in order to use the application."
		);
		err_count++;
	}

	if (this->settings->INSTALLED_MODULES.empty())
	{
		this->settings->LOGGER->warning(
			"You have not added any module to INSTALLED_MODULES setting."
		);
	}

	if (this->settings->SECRET_KEY.empty())
	{
		this->settings->LOGGER->error(
			"SECRET_KEY must be set in order to use the application."
		);
		err_count++;
	}

	for (auto& module : this->settings->INSTALLED_MODULES)
	{
		if (!module->ready())
		{
			this->settings->LOGGER->error("Module '" + module->get_name() + "' is not ready.");
			err_count++;
			break;
		}
	}

	if (err_count > 0)
	{
		throw core::ImproperlyConfigured(
			"System check identified " + std::to_string(err_count) + " issues."
		);
	}

	std::cout << "Done." << std::endl;
}

net::HandlerFunc MainApplication::make_handler()
{
	// Check if static files can be served
	// and create necessary urls.
	this->build_static_patterns(this->settings->ROOT_URLCONF);

	// Retrieve main module patterns and append them
	// to result.
	this->build_module_patterns(this->settings->ROOT_URLCONF);

	// Initialize template engine's libraries.
	this->settings->TEMPLATE_ENGINE->load_libraries();

	auto handler = [this](
		net::RequestContext* ctx, const collections::Dict<std::string, std::string>& env
	) -> uint
	{
		auto request = this->make_request(ctx, env);
		core::Result<std::shared_ptr<http::IHttpResponse>> result;
		try
		{
			result = this->process_request_middleware(request);
			if (!result.catch_(core::HttpError) && !result.value)
			{
				result = this->process_urlpatterns(request, this->settings->ROOT_URLCONF);
				// TODO: check if it is required to process response middleware in case of view error.
				if (!result.catch_(core::HttpError))
				{
					if (!result.value)
					{
						// If view returns empty result, return 204 - No Content.
						result.value = std::make_shared<http::HttpResponse>(204);
						this->settings->LOGGER->warning(
							"Response was not instantiated, returned 204",
							_ERROR_DETAILS_
						);
					}
					else
					{
						if (!result.value->err())
						{
							auto middleware_result = this->process_response_middleware(
								request, result.value
							);
							if (middleware_result.catch_(core::HttpError) || middleware_result.value)
							{
								if (middleware_result.err)
								{
									this->settings->LOGGER->trace(
										"Method 'process_response_middleware' returned an error", _ERROR_DETAILS_
									);
								}

								result = middleware_result;
							}
						}
						else
						{
							this->settings->LOGGER->trace(
								"IHttpResponse contains error", _ERROR_DETAILS_
							);
						}
					}
				}
				else
				{
					this->settings->LOGGER->trace(
						"Function 'process_urlpatterns' returned an error", _ERROR_DETAILS_
					);
				}
			}
			else
			{
				this->settings->LOGGER->trace(
					"Method 'process_request_middleware' returned an error", _ERROR_DETAILS_
				);
			}
		}
		catch (const core::BaseException& exc)
		{
			this->settings->LOGGER->trace(
				"An error was caught as core::BaseException", _ERROR_DETAILS_
			);
			result = core::raise<
				core::InternalServerError, std::shared_ptr<http::IHttpResponse>
			>(exc.what());
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->trace(
				"An error was caught as std::exception", _ERROR_DETAILS_
			);
			result = core::raise<core::InternalServerError, std::shared_ptr<http::IHttpResponse>>(
				"<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" + std::string(exc.what()) + "</p>"
			);
		}

		return start_response(ctx, result);
	};

	return handler;
}

bool MainApplication::static_is_allowed(const std::string& static_url)
{
	auto parser = parsers::url_parser();
	parser.parse(static_url);

	// Allow serving local static files if debug and
	// static url is local.
	return this->settings->DEBUG && parser.hostname.empty();
}

void MainApplication::build_static_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns)
{
	if (!this->settings->STATIC_ROOT.empty() && this->static_is_allowed(this->settings->STATIC_URL))
	{
		patterns.push_back(urls::make_static(this->settings->STATIC_URL, this->settings->STATIC_ROOT));
	}

	if (!this->settings->MEDIA_ROOT.empty() && this->static_is_allowed(this->settings->MEDIA_URL))
	{
		patterns.push_back(
			urls::make_static(this->settings->MEDIA_URL, this->settings->MEDIA_ROOT, "media")
		);
	}
}

void MainApplication::build_module_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns)
{
	if (this->settings->ROOT_MODULE)
	{
		auto modules_patterns = this->settings->ROOT_MODULE->get_urlpatterns();
		patterns.insert(patterns.end(), modules_patterns.begin(), modules_patterns.end());
	}
}

core::Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_request_middleware(
	std::shared_ptr<http::HttpRequest>& request
)
{
	for (auto& middleware : this->settings->MIDDLEWARE)
	{
		auto result = middleware->process_request(request.get());
		if (result.err)
		{
			// TODO: print middleware name.
			this->settings->LOGGER->trace(
				"Method 'process_request' of 'unknown' middleware returned an error", _ERROR_DETAILS_
			);
			return result;
		}
	}

	return core::Result<std::shared_ptr<http::IHttpResponse>>::null();
}

core::Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_urlpatterns(
	std::shared_ptr<http::HttpRequest>& request,
	std::vector<std::shared_ptr<urls::UrlPattern>>& urlpatterns
)
{
	auto apply = urls::resolve(request->path(), this->settings->ROOT_URLCONF);
	if (apply)
	{
		return apply(request.get(), this->settings);
	}

	this->settings->LOGGER->trace("The requested resource was not found", _ERROR_DETAILS_);
	return core::raise<
		core::NotFound, std::shared_ptr<http::IHttpResponse>
	>("<h2>404 - Not Found</h2>");
}

core::Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_response_middleware(
	std::shared_ptr<http::HttpRequest>& request,
	std::shared_ptr<http::IHttpResponse>& response
)
{
	long long size = this->settings->MIDDLEWARE.size();
	for (long long i = size - 1; i >= 0; i--)
	{
		auto result = this->settings->MIDDLEWARE[i]->process_response(
			request.get(), response.get()
		);
		if (result.err)
		{
			// TODO: print middleware name.
			this->settings->LOGGER->trace(
				"Method 'process_response' of 'unknown' middleware returned an error", _ERROR_DETAILS_
			);
			return result;
		}
	}

	return core::Result<std::shared_ptr<http::IHttpResponse>>::null();
}

// TODO: fill the table below.
// | METHOD     | Contains request body? | Contains response body? |
// +------------+------------------------+-------------------------+
// | OPTIONS    |                        |                         |
// | GET        |                        |                         |
// | HEAD       |                        |                         |
// | POST       |                        |                         |
// | PUT        |                        |                         |
// | DELETE     |                        |                         |
// | TRACE      |                        |                         |
// | CONNECT    |                        |                         |
// | PATCH      |                        |                         |
std::shared_ptr<http::HttpRequest> MainApplication::make_request(
	net::RequestContext* ctx, collections::Dict<std::string, std::string> env
)
{
	parsers::query_parser qp;
	http::HttpRequest::Parameters<std::string, xw::string> get_params, post_params;
	http::HttpRequest::Parameters<std::string, files::UploadedFile> files_params;
	if (ctx->content_size)
	{
		auto cont_type = str::lower(ctx->headers.get("Content-Type"));
		if (cont_type == "application/x-www-form-urlencoded")
		{
			qp.parse(ctx->content);
			if (ctx->method == "GET")
			{
				get_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
			}
			else if (ctx->method == "POST")
			{
				post_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
			}
		}
		else if (cont_type == "multipart/form-data")
		{
			parsers::multipart_parser mp(this->settings->MEDIA_ROOT);
			mp.parse(ctx->headers["Content-Type"], ctx->content);
			post_params = http::HttpRequest::Parameters(mp.post_values, mp.multi_post_value);
			files_params = http::HttpRequest::Parameters(mp.file_values, mp.multi_file_value);
		}
	}
	else
	{
		qp.parse(ctx->query);
		if (ctx->method == "GET")
		{
			get_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
		}
		else if (ctx->method == "POST")
		{
			post_params = http::HttpRequest::Parameters(qp.dict, qp.multi_dict);
		}
	}

	for (const auto& header : ctx->headers)
	{
		auto key = str::replace(header.first, "-", "_");
		env["HTTP_" + str::upper(key)] = header.second;
	}

	return std::make_shared<http::HttpRequest>(
		this->settings,
		ctx->method,
		ctx->path,
		ctx->major_v,
		ctx->minor_v,
		ctx->query,
		ctx->keep_alive,
		ctx->content,
		ctx->headers,
		get_params,
		post_params,
		files_params,
		env
	);
}

std::shared_ptr<http::IHttpResponse> MainApplication::error_to_response(const core::Error* err)
{
	unsigned short code;
	switch (err->type)
	{
		case core::EntityTooLargeError:
			code = 413;
			break;
		case core::PermissionDenied:
			code = 403;
			break;
		case core::NotFound:
		case core::FileDoesNotExistError:
			code = 404;
			break;
		case core::RequestTimeout:
			code = 408;
			break;
		case core::InternalServerError:
			code = 500;
			break;
		case core::SuspiciousOperation:
		case core::DisallowedHost:
		case core::DisallowedRedirect:
			code = 400;
			break;
		case core::HttpError:
		default:
			code = 500;
			break;
	}

	// TODO: "<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" +  + "</p>"
	return std::make_shared<http::HttpResponse>(code, err->msg);
}

uint MainApplication::start_response(
	net::RequestContext* ctx,
	const core::Result<std::shared_ptr<http::IHttpResponse>>& result
)
{
	std::shared_ptr<http::IHttpResponse> response;
	if (result.catch_(core::HttpError))
	{
		this->settings->LOGGER->trace(result.err.msg, _ERROR_DETAILS_);
		response = error_to_response(&result.err);
	}
	else if (!result.value)
	{
		// Response was not instantiated, so return 204 - No Content.
		response = std::make_shared<http::HttpResponse>(204);
		this->settings->LOGGER->warning(
				"Response was not instantiated, returned 204",
				_ERROR_DETAILS_
		);
	}
	else
	{
		auto error = result.value->err();
		if (error)
		{
			this->settings->LOGGER->trace(error.msg, _ERROR_DETAILS_);
			response = error_to_response(&error);
		}
		else
		{
			response = result.value;
		}
	}

	this->finish_response(ctx, response.get());
	return response->status();
}

void MainApplication::finish_response(
	net::RequestContext* ctx, http::IHttpResponse* response
)
{
	if (response->is_streaming())
	{
		auto* streaming_response = dynamic_cast<http::StreamingHttpResponse*>(response);
		std::string chunk;
		while (!(chunk = streaming_response->get_chunk()).empty())
		{
			if (!ctx->write(chunk.c_str(), chunk.size()))
			{
				this->settings->LOGGER->trace("Unable to send chunk", _ERROR_DETAILS_);
			}
		}

		response->close();
	}
	else
	{
		auto data = response->serialize();
		if (!ctx->write(data.c_str(), data.size()))
		{
			this->settings->LOGGER->trace("Unable to send response", _ERROR_DETAILS_);
		}
	}
}

__APPS_END__
