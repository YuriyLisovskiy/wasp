/**
 * management/commands/runserver.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./start_server.h"

// Core libraries.
#include <xalwart.core/string_utils.h>
#include <xalwart.core/result.h>
#include <xalwart.core/datetime.h>

// Framework libraries.
#include "../../urls/resolver.h"
#include "../../core/parsers/url_parser.h"


__MANAGEMENT_COMMANDS_BEGIN__

StartServerCommand::StartServerCommand(
	apps::IAppConfig* config, conf::Settings* settings
) : AppCommand(config, settings, "start-server", "Starts a web application")
{
	std::string ipv4 = R"((\d{1,3}(?:\.\d{1,3}){3})|localhost)";
	this->_ipv4_regex = rgx::Regex(ipv4);

	std::string ipv6 = R"(\[([a-fA-F0-9:]+)\]|\[localhost\])";
	this->_ipv6_regex = rgx::Regex(ipv6);

	std::string fqdn = R"([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*)";

	std::string port = R"(\d+)";
	this->_ipv4_ipv6_port_regex = rgx::Regex(
		R"((?:(()" + ipv4 + R"()|()" + ipv6 + R"()|()" + fqdn + R"()):)?()" + port + R"())"
	);
}

collections::Dict<std::string, std::string> StartServerCommand::get_kwargs()
{
	auto kwargs = AppCommand::get_kwargs();
	kwargs["bind"] = this->_host + ":" + std::to_string(this->_port);
	kwargs["host"] = this->_host;
	kwargs["port"] = std::to_string(this->_port);
	kwargs["threads"] = std::to_string(this->_threads_count);
	kwargs["use-ipv6"] = std::to_string(this->_use_ipv6);
	return kwargs;
}

void StartServerCommand::add_flags()
{
	this->_addr_port_flag = this->flag_set->make_string(
		"bind", "", "Server address and port formatted as host:port"
	);
	this->_addr_flag = this->flag_set->make_string(
		"host", "", "Server address"
	);
	this->_port_flag = this->flag_set->make_uint16(
		"port", this->DEFAULT_PORT, "Server port"
	);
	this->_threads_flag = this->flag_set->make_unsigned_long(
		"threads", this->DEFAULT_THREADS, "Threads count"
	);
	this->_use_ipv6_flag = this->flag_set->make_bool(
		"use-ipv6", false, "Use IPv6 address or not (used in case when host is set to 'localhost')"
	);
}

void StartServerCommand::handle()
{
	if (!this->settings->DEBUG && this->settings->ALLOWED_HOSTS.empty())
	{
		throw core::CommandError("You must set 'allowed_hosts' if 'debug' is false.");
	}

	this->retrieve_args(this->_host, this->_port, this->_use_ipv6, this->_threads_count);
	auto server = this->settings->use_server(
		this->make_handler(),
		this->get_kwargs()
	);
	if (!server)
	{
		throw core::ImproperlyConfigured("You must initialize server in order to use the application.");
	}

	try
	{
		if (server->bind(this->_host.c_str(), this->_port, this->_use_ipv6))
		{
			xw::core::InterruptException::initialize();
			try
			{
				std::string message = dt::Datetime::now().strftime("%B %d, %Y - %T") + "\n" +
				                      LIB_NAME + " version " + LIB_VERSION + "\n" +
				                      "Starting development server at " +
				                      "http://" + this->_host + ":" + std::to_string(this->_port) + "/\n" +
				                      "Quit the server with CONTROL-C.\n";
				server->listen(message);
			}
			catch (const xw::core::InterruptException& exc)
			{
				// skip
			}
		}
	}
	catch (const core::InterruptException& exc)
	{
		this->settings->LOGGER->debug("Interrupted");
	}
	catch (const core::BaseException& exc)
	{
		this->settings->LOGGER->error(exc);
	}
	catch (const std::exception& exc)
	{
		this->settings->LOGGER->error(exc.what(), _ERROR_DETAILS_);
	}

	server->close();
}

std::function<
	core::Result<std::shared_ptr<http::IHttpResponse>>(http::HttpRequest*, const int&)
> StartServerCommand::make_handler()
{
	// Check if static files can be served
	// and create necessary urls.
	this->build_static_patterns(this->settings->ROOT_URLCONF);

	// Retrieve main app patterns and append them
	// to result.
	this->build_app_patterns(this->settings->ROOT_URLCONF);

	// Initialize template engine's libraries.
	this->settings->TEMPLATES_ENGINE->load_libraries();

	auto handler = [this](
		http::HttpRequest* request, const int& client
	) mutable -> core::Result<std::shared_ptr<http::IHttpResponse>>
	{
		core::Result<std::shared_ptr<http::IHttpResponse>> result;
		try
		{
			result = StartServerCommand::process_request_middleware(
				request, this->settings
			);
			if (!result.catch_(core::HttpError) && !result.value)
			{
				result = StartServerCommand::process_urlpatterns(
					request, this->settings->ROOT_URLCONF, this->settings
				);
				// TODO: check if it is required to process response middleware in case of view error.
				if (!result.catch_(core::HttpError))
				{
					if (!result.value)
					{
						// If view returns empty result, return 204 - No Content.
						result.value = std::make_shared<http::HttpResponse>(204);
					}
					else
					{
						if (!result.value->err())
						{
							auto middleware_result = StartServerCommand::process_response_middleware(
								request, result.value.get(), this->settings
							);
							if (middleware_result.catch_(core::HttpError) || middleware_result.value)
							{
								result = middleware_result;
							}
						}
					}
				}
			}
		}
		catch (const core::BaseException& exc)
		{
			this->settings->LOGGER->error(exc);
			result = core::raise<core::InternalServerError, std::shared_ptr<http::IHttpResponse>>(
				"<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" + std::string(exc.what()) + "</p>"
			);
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->error(exc.what(), __LINE__, "request handler function", __FILE__);
			result = core::raise<core::InternalServerError, std::shared_ptr<http::IHttpResponse>>(
				"<p style=\"font-size: 24px;\" >Internal Server Error</p><p>" + std::string(exc.what()) + "</p>"
			);
		}

		return result;
	};

	return handler;
}

bool StartServerCommand::static_is_allowed(const std::string& static_url)
{
	auto parser = parsers::url_parser();
	parser.parse(static_url);

	// Allow serving local static files if debug and
	// static url is local.
	return this->settings->DEBUG && parser.hostname.empty();
}

void StartServerCommand::build_static_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns)
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

void StartServerCommand::build_app_patterns(std::vector<std::shared_ptr<urls::UrlPattern>>& patterns)
{
	if (this->settings->ROOT_APP)
	{
		auto apps_patterns = this->settings->ROOT_APP->get_urlpatterns();
		patterns.insert(patterns.end(), apps_patterns.begin(), apps_patterns.end());
	}
}

void StartServerCommand::retrieve_args(
	std::string& host, uint16_t& port, bool& use_ipv6, size_t& threads_count
)
{
	// Setup address and port.
	auto host_port_str = this->_addr_port_flag->get();
	if (!host_port_str.empty())
	{
		if (!this->_ipv4_ipv6_port_regex.match(host_port_str))
		{
			throw core::CommandError(host_port_str + " is not valid host:port pair");
		}

		auto groups = this->_ipv4_ipv6_port_regex.groups();
		auto address = (groups[1].empty() ? this->DEFAULT_IPV4_HOST : groups[1]);
		use_ipv6 = this->_ipv6_regex.match(address);
		auto trimmed_addr = address;
		str::rtrim(trimmed_addr, "]");
		str::ltrim(trimmed_addr, "[");
		if (trimmed_addr == "localhost")
		{
			address = use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}

		host = address;
		port = groups[4].empty() ? this->DEFAULT_PORT : (uint16_t) std::stoi(groups[4]);
	}
	else
	{
		auto address = this->_addr_flag->get();
		if (address.empty())
		{
			use_ipv6 = this->_use_ipv6_flag->get();
			address = use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}
		else
		{
			if (this->_ipv6_regex.match(address))
			{
				use_ipv6 = true;
			}
			else if (!this->_ipv4_regex.match(address))
			{
				throw core::CommandError(this->_addr_flag->get_raw() + " is invalid address");
			}
		}

		auto trimmed_addr = address;
		str::rtrim(trimmed_addr, "]");
		str::ltrim(trimmed_addr, "[");
		if (trimmed_addr == "localhost")
		{
			address = use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}

		host = address;
		if (this->_port_flag->valid())
		{
			port = this->_port_flag->get();
		}
		else
		{
			throw core::CommandError(this->_port_flag->get_raw() + " is invalid port");
		}
	}

	if (!this->_threads_flag->valid())
	{
		throw core::CommandError("threads count is not a valid positive integer: " + this->_threads_flag->get_raw());
	}

	threads_count = this->_threads_flag->get();
}

core::Result<std::shared_ptr<http::IHttpResponse>> StartServerCommand::process_request_middleware(
	http::HttpRequest* request, conf::Settings* settings
)
{
	for (auto& middleware : settings->MIDDLEWARE)
	{
		auto result = middleware->process_request(request);
		if (result.err)
		{
			return result;
		}
	}

	return core::Result<std::shared_ptr<http::IHttpResponse>>::null();
}

core::Result<std::shared_ptr<http::IHttpResponse>> StartServerCommand::process_urlpatterns(
	http::HttpRequest* request,
	std::vector<std::shared_ptr<urls::UrlPattern>>& urlpatterns,
	conf::Settings* settings
)
{
	auto apply = urls::resolve(request->path(), settings->ROOT_URLCONF);
	if (apply)
	{
		return apply(request, settings);
	}

	return core::raise<core::NotFound, std::shared_ptr<http::IHttpResponse>>("<h2>404 - Not Found</h2>");
}

core::Result<std::shared_ptr<http::IHttpResponse>> StartServerCommand::process_response_middleware(
	http::HttpRequest* request,
	http::IHttpResponse* response,
	conf::Settings* settings
)
{
	long long size = settings->MIDDLEWARE.size();
	for (long long i = size - 1; i >= 0; i--)
	{
		auto result = settings->MIDDLEWARE[i]->process_response(request, response);
		if (result.err)
		{
			return result;
		}
	}

	return core::Result<std::shared_ptr<http::IHttpResponse>>::null();
}

__MANAGEMENT_COMMANDS_END__
