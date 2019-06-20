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

#ifndef LEAF_TCP_SERVER_H
#define LEAF_TCP_SERVER_H

#include <cstring>
#include <string>
#include <vector>
#include <thread>
#include <functional>
//#include <exception>

#include "../globals.h"
#include "../utils/logger.h"


__INTERNAL_BEGIN__

#if defined(_WIN32) || defined(_WIN64)
// Windows-specific libs and definitions for working with tcp sockets

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

typedef SOCKET socket_t;
typedef int msg_size_t;

#define WSA_LAST_ERR WSAGetLastError()
#define SOCKET_SEND SD_SEND
#define SOCKET_RECEIVE SD_RECEIVE

#elif defined(__unix__) || defined(__linux__)
// Linux-specific libs and definitions for working with tcp socket

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef int socket_t;
typedef ssize_t msg_size_t;

#define INVALID_SOCKET (socket_t)(-1)
#define SOCKET_ERROR (-1)
#define SOCKET_SEND SHUT_RDWR
#define SOCKET_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR '\n'

#else
#error Library is not supported on this platform

#endif

#define MAX_BUFF_SIZE 1024

typedef std::function<const std::string(const std::string&)> tcpHandler;

class TcpServer
{
private:
	uint16_t _port;
	const char* _host;
	tcpHandler _handler;
	socket_t _socket;
	sockaddr_in _socketAddr{};
	ILogger* _logger;

	void startListener();
	void serveConnection(const socket_t& client);
	void sendResponse(const char* data, const socket_t& connection);
	void closeConnection(const socket_t& sock, const int& how);
	static int closeSocket(const socket_t& socket);
	static void wsaCleanUp();
	void cleanUp(const socket_t& connection);
	std::string recvAll(const socket_t& connection);
	int init();

public:
	struct Context
	{
		const char* host = nullptr;
		uint16_t port = 0;
		tcpHandler handler = nullptr;
		ILogger* logger = nullptr;
	};

	explicit TcpServer(TcpServer::Context ctx);
	void listenAndServe();
	~TcpServer();
};

__INTERNAL_END__


#endif // LEAF_TCP_SERVER_H
