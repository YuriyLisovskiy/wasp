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
#include <fstream>
#include <thread>
#include <poll.h>

#include "../globals.h"
#include "../utils/logger.h"
#include "../conf/defaults.h"
#include "../conf/settings.h"
#include "../core/sockets/server_socket.h"
#include "response.h"
#include "request.h"
#include "parsers/request_parser.h"
#include "../core/exceptions.h"
#include "../core/files/uploaded_file.h"

#include "../utils/datetime/time.h"
#include "../utils/str.h"


__INTERNAL_BEGIN__

#define MAX_BUFF_SIZE 8192 * 8 - 1

typedef std::function<void(HttpRequest&, const socket_t&)> httpHandler;

class HttpServer
{
private:
	const char* _host;
	uint16_t _port;
	const char* _schema;
	size_t _maxBodySize;
	ServerSocket _serverSocket;
	httpHandler _httpHandler;
	bool _finished;
	ILogger* _logger;
	std::string _mediaRoot;

	enum ReadResult
	{
		Continue, Break, None
	};

	int _init();
	void _cleanUp(const socket_t& client);
	HttpRequest _handleRequest(const socket_t& client);
	std::string _readBody(const socket_t& client, const std::string& bodyBeginning, size_t bodyLength);
	std::string _readHeaders(const socket_t& client, std::string& bodyBeginning);
	void _startListener();
	void _serveConnection(const socket_t& client);
	void _threadFunc(const socket_t& client);

	static ReadResult _handleError(char* buffer, int line, const char *function, const char *file);
	static void _send(const char* data, const socket_t& client);
	static bool _setSocketBlocking(int _sock, bool blocking);
	static void _write(const char* data, size_t bytesToWrite, const socket_t& client);
	static void _wsaCleanUp();

public:
	struct Context
	{
		const char* host = nullptr;
		uint16_t port = 0;
		httpHandler handler = nullptr;
		ILogger* logger;
		size_t maxBodySize = 0;
		std::string mediaRoot;
	};

	explicit HttpServer(HttpServer::Context& ctx);
	~HttpServer();
	void finish();
	void listenAndServe();
	static void send(HttpResponseBase* response, const socket_t& client);
	static void send(StreamingHttpResponse* response, const socket_t& client);

private:
	static void _normalizeContext(HttpServer::Context& ctx);
};

__INTERNAL_END__


#endif // WASP_HTTP_SERVER_H
