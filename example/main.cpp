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

#include "../src/core/http_server.h"
#include "../src/middleware/cookies.h"
#include "../src/http/response.h"
#include "../src/views/generic.h"
#include "form_view.h"
#include "../src/core/exceptions.h"
#include "../src/urls/url.h"

// #define DETECT_MEMORY_LEAK
// #include "../tests/unittests/mem_leak_check.h"


void handler(wasp::http::HttpRequest* request, const wasp::internal::socket_t& client)
{
	wasp::middleware::CookieMiddleware().process_request(request);

	std::vector<wasp::urls::UrlPattern> patterns{
		wasp::urls::make_pattern(
			"/profile/<user_id>([0-9]*)/name/<user_name>([A-Za-z]+)/?",
			wasp::views::View::make_view<FormView>(),
			"profile"
		)
	};

	wasp::http::HttpResponse* response = nullptr;
	for (auto& pattern : patterns)
	{
		std::map<std::string, std::string> args_map;
		if (pattern.match(request->path(), args_map))
		{
			response = pattern.apply(request, new wasp::views::Args(args_map), wasp::Logger::get_instance());
			break;
		}
	}

	if (!response)
	{
		response = new wasp::http::HttpResponseNotFound("<h2>404 - Not Found</h2>");
	}

	wasp::internal::HttpServer::send(response, client);
	delete response;
}


int main()
{
	wasp::InterruptException::initialize();

	wasp::internal::HttpServer::context ctx{};
	ctx.handler = handler;
	ctx.port = 8080;
	ctx.max_body_size = 33300000;
	ctx.media_root = "/home/user/Desktop/media/";
	ctx.logger = wasp::Logger::get_instance();

	wasp::internal::HttpServer server(ctx);

	try
	{
		server.listen_and_serve();
	}
	catch (const wasp::InterruptException& exc)
	{
		std::cout << "\nFinishing server...";
		server.finish();
		std::cout << "\nDone.\n";
	}
	catch (const std::exception& exc)
	{
		std::cout << exc.what();
	}

	return 0;
}
