/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of runserver.h.
 */

#include "./runserver.h"


__CORE_COMMANDS_BEGIN__

RunserverCommand::RunserverCommand(apps::IAppConfig* config, conf::Settings* settings)
	: AppCommand(config, settings, "runserver", "Starts Wasp web server")
{
	this->_host_port_flag = nullptr;
	this->_threads_flag = nullptr;
	this->_host_flag = nullptr;
	this->_port_flag = nullptr;

	std::string port = R"(\d+)";
	this->_port_regex = new rgx::Regex(port);

	std::string ipv4 = R"(\d{1,3}(?:\.\d{1,3}){3})";
	this->_ipv4_regex = new rgx::Regex(ipv4);

	std::string ipv6 = R"(\[[a-fA-F0-9:]+\])";
	this->_ipv6_regex = new rgx::Regex(ipv6);

	std::string fqdn = R"([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*)";

	this->_ipv4_ipv6_port_regex = new rgx::Regex(
		R"((?:(()" + ipv4 + R"()|()" + ipv6 + R"()|()" + fqdn + R"()):)?()" + port + R"())"
	);
}

RunserverCommand::~RunserverCommand()
{
	delete this->_ipv4_ipv6_port_regex;
	delete this->_ipv4_regex;
	delete this->_ipv6_regex;
	delete this->_port_regex;
}

void RunserverCommand::add_flags()
{
	this->_host_port_flag = this->_flag_set->make_string(
		"host-port", "", "Server host and port formatted as ip_addr:port"
	);
	this->_host_flag = this->_flag_set->make_string(
		"host", this->DEFAULT_IPV4_HOST, "Server host"
	);
	this->_port_flag = this->_flag_set->make_long(
		"port", this->DEFAULT_PORT, "Server port"
	);
	this->_threads_flag = this->_flag_set->make_long(
		"threads", this->DEFAULT_THREADS, "Tells Wasp how many threads to use"
	);
}

void RunserverCommand::handle()
{
	core::InterruptException::initialize();
	if (!this->settings->DEBUG && this->settings->ALLOWED_HOSTS.empty())
	{
		throw CommandError("You must set settings.ALLOWED_HOSTS if settings.DEBUG is false.");
	}

	core::internal::HttpServer::context ctx{};

	// TODO: remove!
	ctx.verbose = true;

	this->setup_server_ctx(ctx);

	core::internal::HttpServer server(ctx);
	try
	{
		server.listen_and_serve();
	}
	catch (const core::InterruptException& exc)
	{
		server.finish();
		this->settings->LOGGER->debug("Finished");
	}
	catch (const core::BaseException& exc)
	{
		this->settings->LOGGER->error(exc);
	}
	catch (const std::exception& exc)
	{
		this->settings->LOGGER->error(exc.what(), _ERROR_DETAILS_);
	}
}

std::function<void(http::HttpRequest*, const core::internal::socket_t&)>
RunserverCommand::get_handler()
{
	std::vector<urls::UrlPattern> urlpatterns;

	// Check if static/media files can be served
	//  and create necessary urls.
	this->build_static_patterns(urlpatterns);

	// Retrieve main app patterns and append them
	//  to result.
	this->build_app_patterns(urlpatterns);

	auto* settings = this->settings;
	auto log_request = [settings](
		const std::string& info, unsigned short status_code
	)
	{
		if (settings->LOGGER)
		{
			utility::Logger::Color color = utility::Logger::Color::GREEN;
			if (status_code >= 400)
			{
				color = utility::Logger::Color::YELLOW;
			}
			else if (status_code >= 500)
			{
				color = utility::Logger::Color::RED;
			}

			settings->LOGGER->print(str::format(
				"[{0!s}] \"{1!s}\" {2!d}",
				dt::now().strftime("%d/%b/%Y %T").c_str(),
				info.c_str(),
				status_code
			), color);
		}
	};

	auto handler = [settings, urlpatterns, log_request](
		http::HttpRequest* request, const core::internal::socket_t& client
	) mutable -> void
	{
		for (auto& middleware : settings->MIDDLEWARE)
		{
			middleware->process_request(request);
		}

		http::HttpResponseBase* response = nullptr;
		for (auto& url_pattern : urlpatterns)
		{
			std::map<std::string, std::string> args_map;
			if (url_pattern.match(request->path(), args_map))
			{
				response = url_pattern.apply(request, new views::Args(args_map), settings->LOGGER);
				break;
			}
		}

		if (!response)
		{
			response = new http::HttpResponseNotFound("<h2>404 - Not Found</h2>");
		}

		for (long int i = (long int) settings->MIDDLEWARE.size() - 1; i >= 0; i--)
		{
			settings->MIDDLEWARE[i]->process_response(request, response);
		}

		if (response->is_streaming())
		{
			auto* streaming_response = dynamic_cast<http::StreamingHttpResponse*>(response);
			core::internal::HttpServer::send(streaming_response, client);
		}
		else
		{
			core::internal::HttpServer::send(response, client);
		}

		log_request(
			request->method() + " " +
			request->path() + " " +
			"HTTP" + (settings->USE_SSL ? "S/" : "/") +
			request->version(),
			response->status()
		);

		delete response;
	};

	return handler;
}

bool RunserverCommand::static_is_allowed(const std::string& static_url)
{
	auto parser = core::internal::url_parser();
	parser.parse(static_url);

	// Forbid serving local static files if not debug or
	//  static url is non-local, i.e. is not url.
	// TODO: check if net location is not empty.
	return this->settings->DEBUG;
}

void RunserverCommand::build_static_patterns(std::vector<urls::UrlPattern>& patterns)
{
	if (!this->settings->STATIC_ROOT.empty() && this->static_is_allowed(this->settings->STATIC_URL))
	{
		patterns.push_back(urls::make_static(this->settings->STATIC_URL, this->settings->STATIC_ROOT));
	}

	if (!this->settings->MEDIA_ROOT.empty() && this->static_is_allowed(this->settings->MEDIA_URL))
	{
		patterns.push_back(urls::make_static(this->settings->MEDIA_URL, this->settings->MEDIA_ROOT));
	}
}

void RunserverCommand::build_app_patterns(std::vector<urls::UrlPattern>& patterns)
{
	if (!this->settings->INSTALLED_APPS.empty())
	{
		auto apps_patterns = this->settings->INSTALLED_APPS[0]->get_urlpatterns();
		patterns.insert(patterns.end(), apps_patterns.begin(), apps_patterns.end());
	}
}

void RunserverCommand::setup_server_ctx(core::internal::HttpServer::context& ctx)
{
	// Setup host and port.
	auto host_port_str = this->_host_port_flag->get();
	if (!host_port_str.empty())
	{
		if (!this->_ipv4_ipv6_port_regex->match(host_port_str))
		{
			throw CommandError(host_port_str + " is not valid address:port pair");
		}

		auto groups = this->_ipv4_ipv6_port_regex->groups();
		auto address = (groups[1].empty() ? this->DEFAULT_IPV4_HOST : groups[1]);
		if (this->_ipv6_regex->match(address))
		{
			ctx.use_ipv6 = true;
			str::ltrim(address, '[');
			str::rtrim(address, ']');
		}

		ctx.host = address;
		ctx.port = groups[2].empty() ? this->DEFAULT_PORT : (uint16_t) std::stoi(groups[2]);
	}
	else
	{
		auto address = this->_host_flag->get();
		if (address.empty())
		{
			address = this->DEFAULT_IPV4_HOST;
		}
		else
		{
			if (this->_ipv6_regex->match(address))
			{
				ctx.use_ipv6 = true;
				str::ltrim(address, '[');
				str::rtrim(address, ']');
			}
			else if (!this->_ipv4_regex->match(address))
			{
				throw CommandError(this->_host_flag->get_raw() + " is invalid address");
			}
		}

		ctx.host = address.empty() ? (
			ctx.use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST
		) : address;
		auto raw_port = this->_port_flag->get_raw();
		if (raw_port.empty())
		{
			ctx.port = this->DEFAULT_PORT;
		}
		else
		{
			if (!this->_port_regex->match(raw_port))
			{
				throw CommandError(raw_port + " is invalid port");
			}

			ctx.port = this->_port_flag->get();
		}
	}

	ctx.max_body_size = this->settings->DATA_UPLOAD_MAX_MEMORY_SIZE;
	ctx.media_root = this->settings->MEDIA_ROOT;
	ctx.logger = this->settings->LOGGER;

	if (std::regex_match(this->_threads_flag->get_raw(), std::regex(R"(\d+)")))
	{
		throw CommandError("threads count is not a number: " + this->_threads_flag->get_raw());
	}

	ctx.threads_count = this->_threads_flag->get();
	ctx.handler = this->get_handler();
}

__CORE_COMMANDS_END__
