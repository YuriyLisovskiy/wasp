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

/*
 * http server definition.
 * TODO: write docs
 */

#ifndef WASP_HTTP_SERVER_H
#define WASP_HTTP_SERVER_H

#include <iostream>
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

#include "../globals.h"
#include "../utils/logger.h"
#include "../conf/defaults.h"
#include "../conf/settings.h"
#include "../core/tcp_server.h"
#include "response.h"
#include "request.h"
#include "parsers/request_parser.h"
#include "../core/exceptions.h"

#include "../utils/datetime/time.h"
#include "../utils/str.h"


__INTERNAL_BEGIN__

typedef std::function<HttpResponseBase* (HttpRequest&)> httpHandler;

class HttpServer
{
private:
	const char* _host;
	uint16_t _port;
	const char* _schema;

	ILogger* _logger;

	TcpServer* _tcpServer;
	httpHandler _httpHandler;

	void _tcpHandler(const std::string& data, const socket_t& client);

public:
	struct Context
	{
		const char* host = nullptr;
		uint16_t port = 0;
		httpHandler handler = nullptr;
		ILogger* logger;
	};

	explicit HttpServer(HttpServer::Context& ctx);
	void listenAndServe();
	void finish();
	~HttpServer();

private:
	static void normalizeContext(HttpServer::Context& ctx);
};

__INTERNAL_END__


#endif // WASP_HTTP_SERVER_H
