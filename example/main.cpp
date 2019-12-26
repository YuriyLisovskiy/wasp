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

#include <iostream>
#include <string>
#include <vector>
#include <functional>

#include "../src/core/http_server.h"
#include "./example_app/settings.h"


std::function<void(wasp::http::HttpRequest*, const wasp::core::internal::socket_t&)>
make_handler(wasp::conf::Settings* settings)
{
	std::vector<wasp::urls::UrlPattern> urlpatterns{
		wasp::urls::make_static(settings->STATIC_URL, settings->STATIC_ROOT)
	};
	if (!settings->INSTALLED_APPS.empty())
	{
		auto apps_patterns = settings->INSTALLED_APPS[0]->get_urlpatterns();
		urlpatterns.insert(urlpatterns.end(), apps_patterns.begin(), apps_patterns.end());
	}

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

int main()
{
	wasp::core::InterruptException::initialize();

	auto* settings = new Settings();

	wasp::core::internal::HttpServer::context ctx{};
	ctx.handler = make_handler(settings);
	ctx.port = 8000;
	ctx.max_body_size = settings->DATA_UPLOAD_MAX_MEMORY_SIZE;
	ctx.media_root = settings->MEDIA_ROOT;
	ctx.logger = settings->LOGGER;
	ctx.threads_count = settings->QUEUE_THREADS_COUNT;

	wasp::core::internal::HttpServer server(ctx);

	try
	{
		server.listen_and_serve();
	}
	catch (const wasp::core::InterruptException& exc)
	{
		std::cout << "\nFinishing server...";
		server.finish();
		std::cout << "\nDone.\n";
	}
	catch (const std::exception& exc)
	{
		std::cout << exc.what();
	}

	delete settings;
	return 0;
}
