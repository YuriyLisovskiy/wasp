/**
 * conf/xalwart.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include <utility>

#include "./xalwart.h"

// Framework libraries.
#include "../management/module.h"
#include "../http/url.h"
#include "../http/internal/multipart_parser.h"
#include "../urls/resolver.h"
#include "../urls/utilities.h"


__CONF_BEGIN__

MainApplication::MainApplication(
	const std::string& version,
	conf::Settings* settings,
	std::function<std::shared_ptr<net::abc::IServer>(
		log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
	)> server_initializer
) : server_initializer(std::move(server_initializer)), version(version)
{
	InterruptException::initialize();
	if (!settings)
	{
		throw ImproperlyConfigured(
			"Settings must be configured in order to use the application.", _ERROR_DETAILS_
		);
	}

	this->settings = settings;
	this->settings->prepare();
	this->settings->perform_checks();

	this->_setup_commands();

	size_t max_len = 0;
	for (auto& command : this->_commands)
	{
		auto new_len = command.second->name().size();
		if (new_len > max_len)
		{
			max_len = new_len;
		}
	}

	for (auto& command : this->_commands)
	{
		std::string indent(max_len - command.second->name().size(), ' ');
		this->_help_message += "  " + command.second->name() + indent + "  " + command.second->help_message() + '\n';
	}

	if (!this->_help_message.empty())
	{
		this->_help_message = "Usage:\n  application [command]\n\n"
			"Available Commands:\n" + this->_help_message +
			"\nUse \"application [command] --help\" for more information about a command.";
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
		catch (const BaseException& exc)
		{
			this->settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->error(exc.what());
		}
		catch (...)
		{
			this->settings->LOGGER->error("xw::conf::MainApplication: unknown error", _ERROR_DETAILS_);
		}
	}
	else
	{
		std::cout << this->_help_message;
	}
}

void MainApplication::_setup_commands()
{
	// Retrieve commands from 'MODULES' and check if modules'
	// commands do not override settings commands.
	for (auto& installed_module : this->settings->MODULES)
	{
		this->_extend_settings_commands_or_error(
			installed_module->get_commands(),
			[installed_module](const std::string& cmd_name) -> std::string {
				return "Module with name '" + installed_module->get_name() +
					"' overrides commands with '" + cmd_name + "' command";
			}
		);
	}

	auto default_commands = mgmt::CoreModuleConfig(
		this->settings, [this](
			log::ILogger* logger, const Kwargs& kwargs, std::shared_ptr<dt::Timezone> tz
		) -> std::shared_ptr<net::abc::IServer>
		{
			auto server = this->server_initializer(logger, kwargs, std::move(tz));
			server->setup_handler(this->make_handler());
			return server;
		}
	).get_commands();

	// Check if user-defined commands do not override
	// default commands, if not, append them to '_commands'.
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
			this->_commands.begin(), this->_commands.end(),
			[command](const std::pair<std::string, std::shared_ptr<cmd::BaseCommand>>& pair) -> bool {
				return command->name() == pair.first;
			}
		) != this->_commands.end())
		{
			this->settings->LOGGER->warning(err_fn(command->name()));
		}

		this->_commands[command->name()] = command;
	}
}

net::HandlerFunc MainApplication::make_handler()
{
	// Check if static files can be served and create necessary urls.
	this->build_static_patterns(this->settings->URLPATTERNS);

	// Retrieve main module patterns and append them to result.
	this->build_module_patterns(this->settings->URLPATTERNS);

	// Initialize template engine's libraries.
	this->settings->TEMPLATE_ENGINE->load_libraries();

	auto handler = [this](
		net::RequestContext* ctx, const collections::Dictionary<std::string, std::string>& env
	) -> uint
	{
		auto request = this->make_request(ctx, env);
		Result<std::shared_ptr<http::IHttpResponse>> result;
		try
		{
			result = this->process_request_middleware(request);
			if (!result.catch_(HttpError) && !result.value)
			{
				result = this->process_urlpatterns(request, this->settings->URLPATTERNS);

				// TODO: check if it is required to process response middleware in case of controller error.
				if (!result.catch_(HttpError))
				{
					if (!result.value)
					{
						// If controller returns empty result, return 204 - No Content.
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
							if (middleware_result.catch_(HttpError) || middleware_result.value)
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
		catch (const BaseException& exc)
		{
			this->settings->LOGGER->trace("An error was caught as core::BaseException", _ERROR_DETAILS_);
			result = raise<InternalServerError, std::shared_ptr<http::IHttpResponse>>(exc.what());
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->trace("An error was caught as std::exception", _ERROR_DETAILS_);
			result = raise<InternalServerError, std::shared_ptr<http::IHttpResponse>>(
				"<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" + std::string(exc.what()) + "</p>"
			);
		}

		return start_response(ctx, result);
	};

	return handler;
}

bool MainApplication::static_is_allowed(const std::string& static_url)
{
	auto url = http::parse_url(static_url);

	// Allow serving local static files if debug and
	// static url is local.
	return this->settings->DEBUG && url.hostname().empty();
}

void MainApplication::build_static_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns)
{
	if (!this->settings->STATIC_ROOT.empty() && this->static_is_allowed(this->settings->STATIC_URL))
	{
		patterns.push_back(urls::make_static(this->settings->STATIC_URL, this->settings->STATIC_ROOT));
	}

	if (!this->settings->MEDIA_ROOT.empty() && this->static_is_allowed(this->settings->MEDIA_URL))
	{
		patterns.push_back(urls::make_static(this->settings->MEDIA_URL, this->settings->MEDIA_ROOT, "media"));
	}
}

void MainApplication::build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns)
{
	if (!this->settings->MODULES.empty())
	{
		auto root_module = this->settings->MODULES.front();
		if (!root_module)
		{
			throw NullPointerException("'root_module' is nullptr", _ERROR_DETAILS_);
		}

		auto modules_patterns = root_module->get_urlpatterns();
		patterns.insert(patterns.end(), modules_patterns.begin(), modules_patterns.end());
	}
}

Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_request_middleware(
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

	return Result<std::shared_ptr<http::IHttpResponse>>::null();
}

Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_urlpatterns(
	std::shared_ptr<http::HttpRequest>& request,
	std::vector<std::shared_ptr<urls::IPattern>>& urlpatterns
)
{
	auto apply = urls::resolve(request->path(), this->settings->URLPATTERNS);
	if (apply)
	{
		return apply(request.get(), this->settings);
	}

	this->settings->LOGGER->trace("The requested resource was not found", _ERROR_DETAILS_);
	return raise<NotFound, std::shared_ptr<http::IHttpResponse>>("<h2>404 - Not Found</h2>");
}

Result<std::shared_ptr<http::IHttpResponse>> MainApplication::process_response_middleware(
	std::shared_ptr<http::HttpRequest>& request,
	std::shared_ptr<http::IHttpResponse>& response
)
{
	long long size = (long long)this->settings->MIDDLEWARE.size();
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

	return Result<std::shared_ptr<http::IHttpResponse>>::null();
}

std::shared_ptr<http::HttpRequest> MainApplication::make_request(
	net::RequestContext* ctx, collections::Dictionary<std::string, std::string> env
)
{
	collections::MultiDictionary<std::string, std::string> get_params, post_params;
	collections::MultiDictionary<std::string, files::UploadedFile> files_params;
	if (ctx->content_size)
	{
		auto cont_type = str::lower(ctx->headers.get("Content-Type"));
		if (cont_type.starts_with("application/x-www-form-urlencoded"))
		{
			auto query = http::parse_query(ctx->content);
			if (ctx->method == "GET")
			{
				get_params = query;
			}
			else if (ctx->method == "POST")
			{
				post_params = query;
			}
		}
		else if (cont_type.starts_with("multipart/form-data"))
		{
			http::internal::multipart_parser mp(this->settings->MEDIA_ROOT);
			mp.parse(ctx->headers.get("Content-Type"), ctx->content);
			post_params = mp.multi_post_value;
			files_params = mp.multi_file_value;
		}
	}
	else
	{
		auto query = http::parse_query(ctx->query);
		if (ctx->method == "GET")
		{
			get_params = query;
		}
		else if (ctx->method == "POST")
		{
			post_params = query;
		}
	}

	for (const auto& header : ctx->headers)
	{
		auto key = str::replace(header.first, "-", "_");
		env.set("HTTP_" + str::upper(key), header.second);
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

std::shared_ptr<http::IHttpResponse> MainApplication::error_to_response(const Error* err)
{
	unsigned short code;
	switch (err->type)
	{
		case EntityTooLargeError:
			code = 413;
			break;
		case PermissionDenied:
			code = 403;
			break;
		case NotFound:
		case FileDoesNotExistError:
			code = 404;
			break;
		case RequestTimeout:
			code = 408;
			break;
		case InternalServerError:
			code = 500;
			break;
		case SuspiciousOperation:
		case DisallowedHost:
		case DisallowedRedirect:
			code = 400;
			break;
		case HttpError:
		default:
			code = 500;
			break;
	}

	// TODO: "<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" +  + "</p>"
	return std::make_shared<http::HttpResponse>(code, err->msg);
}

uint MainApplication::start_response(
	net::RequestContext* ctx,
	const Result<std::shared_ptr<http::IHttpResponse>>& result
)
{
	std::shared_ptr<http::IHttpResponse> response;
	if (result.catch_(HttpError))
	{
		this->settings->LOGGER->trace(result.err.msg, _ERROR_DETAILS_);
		response = error_to_response(&result.err);
	}
	else if (!result.value)
	{
		// Response was not instantiated, so return 204 - No Content.
		response = std::make_shared<http::HttpResponse>(204);
		this->settings->LOGGER->warning(
			"Response was not instantiated, returned 204", _ERROR_DETAILS_
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

__CONF_END__
