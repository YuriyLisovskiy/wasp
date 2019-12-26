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

RunserverCommand::RunserverCommand(wasp::conf::Settings* settings)
	: Command(settings, "runserver", "Starts Wasp web server")
{
	this->_host_port_flag = nullptr;
	this->_threads_flag = nullptr;
	this->_host_flag = nullptr;
	this->_port_flag = nullptr;
}

void RunserverCommand::add_flags()
{
	this->_host_port_flag = this->_flag_set->make_string(
		"host-port", "", "Server host and port formatted as ip_addr:port"
	);
	this->_host_flag = this->_flag_set->make_string(
		"host", "127.0.0.1", "Server host"
	);
	this->_port_flag = this->_flag_set->make_long(
		"port", 8000, "Server port"
	);
	this->_threads_flag = this->_flag_set->make_long(
		"threads", 16, "Tells Wasp how many threads to use"
	);
}

void RunserverCommand::handle()
{
	wasp::core::InterruptException::initialize();

	wasp::core::internal::HttpServer::context ctx{};
	auto host_port_str = this->_host_port_flag->get();
	if (!host_port_str.empty())
	{
		auto host_port = str::split(this->_host_port_flag->get(), ':');
		ctx.host = host_port[0].c_str();
		ctx.port = (uint16_t) std::stoi(host_port[1]);
	}
	else
	{
		ctx.host = this->_host_flag->get().c_str();
		ctx.port = this->_port_flag->get();
	}

	ctx.handler = this->get_handler();
	ctx.max_body_size = this->settings->DATA_UPLOAD_MAX_MEMORY_SIZE;
	ctx.media_root = this->settings->MEDIA_ROOT;
	ctx.logger = this->settings->LOGGER;
	ctx.threads_count = this->_threads_flag->get();

	wasp::core::internal::HttpServer server(ctx);
	try
	{
		server.listen_and_serve();
	}
	catch (const wasp::core::InterruptException& exc)
	{
		server.finish();
		this->settings->LOGGER->debug("Finished", _ERROR_DETAILS_);
	}
	catch (const std::exception& exc)
	{
		std::cout << exc.what();
	}
}

std::function<void(http::HttpRequest*, const core::internal::socket_t&)>
RunserverCommand::get_handler()
{
	std::vector<wasp::urls::UrlPattern> urlpatterns{
			wasp::urls::make_static(this->settings->STATIC_URL, this->settings->STATIC_ROOT)
	};
	if (!this->settings->INSTALLED_APPS.empty())
	{
		auto apps_patterns = this->settings->INSTALLED_APPS[0]->get_urlpatterns();
		urlpatterns.insert(urlpatterns.end(), apps_patterns.begin(), apps_patterns.end());
	}

	auto* settings = this->settings;
	auto handler = [settings, urlpatterns](
		wasp::http::HttpRequest* request, const wasp::core::internal::socket_t& client
	) mutable -> void
	{
		std::cout << "\n\n" << request->path();

		for (auto& middleware : settings->MIDDLEWARE)
		{
			middleware->process_request(request);
		}

		wasp::http::HttpResponseBase* response = nullptr;
		for (auto& url_pattern : urlpatterns)
		{
			std::map<std::string, std::string> args_map;
			if (url_pattern.match(request->path(), args_map))
			{
				response = url_pattern.apply(request, new wasp::views::Args(args_map), settings->LOGGER);
				break;
			}
		}

		if (!response)
		{
			response = new wasp::http::HttpResponseNotFound("<h2>404 - Not Found</h2>");
		}

		for (long int i = (long int) settings->MIDDLEWARE.size() - 1; i >= 0; i--)
		{
			settings->MIDDLEWARE[i]->process_response(request, response);
		}

		if (response->is_streaming())
		{
			auto* streaming_response = dynamic_cast<wasp::http::StreamingHttpResponse*>(response);
			wasp::core::internal::HttpServer::send(streaming_response, client);
		}
		else
		{
			wasp::core::internal::HttpServer::send(response, client);
		}

		delete response;
	};

	return handler;
}

__CORE_COMMANDS_END__
