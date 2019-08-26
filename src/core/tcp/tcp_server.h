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
 * tcp/ip server definition.
 */

#ifndef WASP_CORE_TCP_SERVER_H
#define WASP_CORE_TCP_SERVER_H

#include <cstring>
#include <string>
#include <vector>
#include <thread>
#include <functional>
#include <fcntl.h>
#include <poll.h>

// TODO: --------:
#include <chrono>
#include <thread>
// TODO: --------^

#include "../../globals.h"
#include "../../utils/logger.h"
#include "../../conf/defaults.h"
#include "../exceptions.h"
#include "server_socket.h"

__INTERNAL_BEGIN__

#define MAX_BUFF_SIZE 8192 * 8 - 1

typedef std::function<void(const std::string&, const socket_t&)> tcpHandler;

class TcpServer
{
private:
	uint16_t _port;
	const char* _host;
	ServerSocket _serverSocket;
	size_t _maxRequestSize;
	tcpHandler _handler;
	ILogger* _logger;

	enum ReadResult
	{
		Continue, None
	};

	int _init();
	void _cleanUp(const socket_t& connection);
	std::string _recvAll(const socket_t& connection);
	void _serveConnection(const socket_t& client);
	void _startListener();

	static ReadResult HandleError(char* buffer, int& status, int line, const char *function, const char *file);
	static bool SetSocketBlocking(int _sock, bool blocking);
	static void WSACleanUp();

public:
	struct Context
	{
		const char* host = nullptr;
		uint16_t port = 0;
		tcpHandler handler = nullptr;
		ILogger* logger = nullptr;
		size_t maxRequestSize = 0;
	};

	explicit TcpServer(TcpServer::Context ctx);
	~TcpServer();
	void listenAndServe();

	static void Send(const char* data, const socket_t& connection);
	static void Write(const char* data, size_t bytesToWrite, const socket_t& connection);
};

__INTERNAL_END__


#endif // WASP_CORE_TCP_SERVER_H
