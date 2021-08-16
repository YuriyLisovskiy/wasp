/**
 * conf/xalwart.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./xalwart.h"

// Framework libraries.
#include "../http/url.h"
#include "../management/module.h"
#include "../http/exceptions.h"
#include "../http/internal/multipart_parser.h"
#include "../urls/resolver.h"
#include "../urls/utilities.h"


__CONF_BEGIN__

void MainApplication::_setup_commands(net::HandlerFunc handler)
{
	auto core_module = mgmt::CoreModuleConfig(
		this->settings, [this, handler](
			log::ILogger* logger, const Kwargs& kwargs, std::shared_ptr<dt::Timezone> tz
		) mutable -> std::unique_ptr<net::abc::IServer>
		{
			auto server = this->server_initializer(logger, kwargs, std::move(tz));
			server->setup_handler(std::move(handler));
			return server;
		}
	);

	this->_extend_settings_commands(core_module.get_commands(), core_module.get_name());
	for (auto& installed_module : this->settings->MODULES)
	{
		this->_extend_settings_commands(installed_module->get_commands(), installed_module->get_name());
	}
}

void MainApplication::_extend_settings_commands(
	const std::vector<std::shared_ptr<cmd::BaseCommand>>& from, const std::string& module_name
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
			this->settings->LOGGER->warning(
				"Module with name '" + module_name + "' overrides commands with '" + command->name() + "' command"
			);
		}

		this->_commands[command->name()] = command;
	}
}

MainApplication::MainApplication(
	const std::string& version,
	conf::Settings* settings,
	std::function<std::unique_ptr<net::abc::IServer>(
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

	// Check if static files can be served and create necessary urls.
	this->build_static_pattern(
		this->settings->URLPATTERNS, this->settings->STATIC_ROOT, this->settings->STATIC_URL, "static"
	);
	this->build_static_pattern(
		this->settings->URLPATTERNS, this->settings->MEDIA_ROOT, this->settings->MEDIA_URL, "media"
	);

	// Retrieve main module patterns and append them to result.
	this->build_module_patterns(this->settings->URLPATTERNS);

	// Initialize template engine's libraries.
	if (this->settings->TEMPLATE_ENGINE == nullptr)
	{
		throw NullPointerException("'settings->TEMPLATE_ENGINE' is nullptr", _ERROR_DETAILS_);
	}

	this->settings->TEMPLATE_ENGINE->load_libraries();

	this->_setup_commands(this->make_handler());

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

net::HandlerFunc MainApplication::make_handler() const
{
	return [this](net::RequestContext* ctx, const collections::Dictionary<std::string, std::string>& env) -> uint
	{
		auto request = this->build_request(ctx, env);
		http::Response::Result result;
		try
		{
			result = this->process_request(request);
			if (!result.exception && !result.response)
			{
				result = this->process_urlpatterns(request, this->settings->URLPATTERNS);

				// TODO: check if it is required to process response middleware in case of controller error.
				if (!result.exception)
				{
					if (!result.response)
					{
						// If controller returns empty result, return 204 - No Content.
						result.response = std::make_shared<http::Response>(204);
						this->settings->LOGGER->warning("Response was not instantiated, returned 204", _ERROR_DETAILS_);
					}
					else
					{
						auto middleware_result = this->process_response(request, result.response);
						if (middleware_result.exception || middleware_result.response)
						{
							if (middleware_result.exception)
							{
								this->settings->LOGGER->trace(
									"Method 'process_response_middleware' returned an error", _ERROR_DETAILS_
								);
							}

							result = middleware_result;
						}
					}
				}
				else
				{
					this->settings->LOGGER->trace(
						"Method 'process_urlpatterns' returned an error", _ERROR_DETAILS_
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
		catch (const http::exc::HttpError& e)
		{
			this->settings->LOGGER->trace("An error was caught as http::HttpException", _ERROR_DETAILS_);
			result = http::Response::Result{nullptr, std::make_shared<http::exc::HttpError>(e)};
		}
		catch (const BaseException& e)
		{
			this->settings->LOGGER->trace("An error was caught as core::BaseException", _ERROR_DETAILS_);
			result = http::Response::Result{nullptr, std::make_shared<BaseException>(e)};
		}
		catch (const std::exception& e)
		{
			this->settings->LOGGER->error(e.what(), _ERROR_DETAILS_);
			result = http::raise(500, e.what());
		}

		return start_response(ctx, result);
	};
}

bool MainApplication::static_is_allowed(const std::string& static_url) const
{
	auto url = http::parse_url(static_url);

	// Allow serving local static files if debug and
	// static url is local.
	return this->settings->DEBUG && url.hostname().empty();
}

void MainApplication::build_static_pattern(
	std::vector<std::shared_ptr<urls::IPattern>>& patterns,
	const std::string& root, const std::string& url, const std::string& name
) const
{
	if (!root.empty() && this->static_is_allowed(url))
	{
		patterns.push_back(urls::make_static(url, root, name));
	}
}

void MainApplication::build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const
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

http::Response::Result MainApplication::process_request(std::shared_ptr<http::Request>& request) const
{
	for (auto& middleware : this->settings->MIDDLEWARE)
	{
		auto result = middleware->process_request(request.get());
		if (result.exception)
		{
			// TODO: print middleware name.
			this->settings->LOGGER->trace(
				"Method 'process_request' of 'unknown' middleware returned an error", _ERROR_DETAILS_
			);
			return result;
		}
	}

	return {};
}

http::Response::Result MainApplication::process_urlpatterns(
	std::shared_ptr<http::Request>& request, std::vector<std::shared_ptr<urls::IPattern>>& urlpatterns
) const
{
	auto apply = urls::resolve(request->path(), this->settings->URLPATTERNS);
	if (apply)
	{
		return apply(request.get(), this->settings);
	}

	this->settings->LOGGER->trace("The requested resource was not found", _ERROR_DETAILS_);
	return http::raise(404, "<h2>404 - Not Found</h2>");
}

http::Response::Result MainApplication::process_response(
	std::shared_ptr<http::Request>& request, std::shared_ptr<http::abc::IHttpResponse>& response
) const
{
	auto size = (long long)this->settings->MIDDLEWARE.size();
	for (long long i = size - 1; i >= 0; i--)
	{
		auto result = this->settings->MIDDLEWARE[i]->process_response(
			request.get(), response.get()
		);
		if (result.exception)
		{
			// TODO: print middleware name.
			this->settings->LOGGER->trace(
				"Method 'process_response' of 'unknown' middleware returned an error", _ERROR_DETAILS_
			);
			return result;
		}
	}

	return {};
}

std::shared_ptr<http::Request> MainApplication::build_request(
	net::RequestContext* ctx, collections::Dictionary<std::string, std::string> env
) const
{
	collections::MultiDictionary<std::string, std::string> get_params, post_params;
	collections::MultiDictionary<std::string, files::UploadedFile> files_params;
	if (ctx->content_size)
	{
		auto cont_type = ctx->headers.contains("Content-Type") ?
			str::lower(ctx->headers.at("Content-Type")) : "";
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
			http::internal::MultipartParser mp(this->settings->MEDIA_ROOT);
			mp.parse(ctx->headers.contains("Content-Type") ? ctx->headers.at("Content-Type") : "", ctx->content);
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

	return std::make_shared<http::Request>(
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

uint MainApplication::start_response(net::RequestContext* ctx, const http::Response::Result& result) const
{
	std::shared_ptr<http::abc::IHttpResponse> response;
	if (result.exception)
	{
		this->settings->LOGGER->trace(result.exception->get_message(), _ERROR_DETAILS_);
		response = std::make_shared<http::resp::ServerError>(result.exception->get_message());
	}
	else if (!result.response)
	{
		// Response was not instantiated, so return 204 - No Content.
		response = std::make_shared<http::Response>(204);
		this->settings->LOGGER->warning("Response was not instantiated, returned 204.", _ERROR_DETAILS_);
	}
	else
	{
		response = result.response;
	}

	this->finish_response(ctx, response.get());
	return response->status();
}

void MainApplication::finish_response(net::RequestContext* ctx, http::abc::IHttpResponse* response) const
{
	if (response->is_streaming())
	{
		auto* streaming_response = dynamic_cast<http::StreamingResponse*>(response);
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
