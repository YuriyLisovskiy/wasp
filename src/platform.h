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
 * This header contains platform-specific global variables and macros.
 */

#ifndef LEAF_PLATFORM_H
#define LEAF_PLATFORM_H

#include "globals.h"

__INTERNAL_BEGIN__

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>
#include <direct.h>

#pragma comment (lib, "Ws2_32.lib")

#define SOCK SOCKET
#define INVALID_SOCK INVALID_SOCKET
#define WSA_CLEANUP WSACleanup()
#define CLOSE_SOCK(sock) closesocket(sock)
#define WSA_LAST_ERR " failed with error #" << WSAGetLastError() << '\n'
#define SOCK_ERROR SOCKET_ERROR
#define SOCK_SEND SD_SEND
#define SOCK_RECEIVE SD_RECEIVE
#define WSA_STARTUP									\
{													\
	int status;										\
	WSADATA wsaData;								\
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);	\
	if (status != 0)								\
	{												\
		std::cerr << "SERVER ERROR: 'HttpServer::run()': 'WSAStartup(MAKEWORD(2, 2), &wsaData)' failed with error #" << status << '\n';\
		return;										\
	}												\
}
#define MSG_SIZE int

#elif defined(__unix__) || defined(__linux__)

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string>

#define SOCK int
#define INVALID_SOCK (SOCK)(~0)
#define CLOSE_SOCK(sock) close(sock)
#define WSA_CLEANUP /**/
#define SOCK_ERROR (-1)
#define SOCK_SEND SHUT_RDWR
#define SOCK_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR '\n'
#define WSA_STARTUP /**/
#define MSG_SIZE ssize_t

#else
#error Library is not supported on this platform

#endif

__INTERNAL_END__

#endif // LEAF_PLATFORM_H
