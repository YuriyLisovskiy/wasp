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

#include "../src/http/http_server.h"
#include "../src/middleware/cookies.h"
#include "../src/http/response.h"
#include "../src/views/generic.h"
#include "form_view.h"
#include "../src/core/exceptions.h"

using wasp::internal::HttpServer;


void handler(wasp::HttpRequest* request, const wasp::internal::socket_t& client)
{
	wasp::CookieMiddleware().processRequest(request);

	auto view = wasp::View::make_view<FormView>();

	auto* response = view(request, nullptr, wasp::Logger::get_instance());

	if (!response)
	{
		response = new wasp::HttpResponseServerError("<h2>500 - Internal Server Error</h2>");
	}

	HttpServer::send(response, client);
	delete response;
}


int main()
{
	wasp::InterruptException::initialize();

	HttpServer::context ctx{};
	ctx.handler = handler;
	ctx.port = 3000;
	ctx.max_body_size = 33300000;
	ctx.media_root = "/home/yuriylisovskiy/Desktop/media/";
	ctx.logger = wasp::Logger::get_instance();

	HttpServer server(ctx);

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
