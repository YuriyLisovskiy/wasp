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
 * socket.h
 * Purpose: tcp/ip socket's wrapper.
 */

#ifndef WASP_CORE_TCP_SOCKET_H
#define WASP_CORE_TCP_SOCKET_H

#include <cstring>
#include <fcntl.h>

#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

#elif defined(__unix__) || defined(__linux__)

#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#else
#error Library is not supported on this platform

#endif

#include "../../globals.h"
#include "../exceptions.h"

__INTERNAL_BEGIN__

#if defined(_WIN32) || defined(_WIN64)

typedef SOCKET socket_t;
typedef int msg_size_t;

#define WSA_LAST_ERR WSAGetLastError()
#define SOCKET_SEND SD_SEND
#define SOCKET_RECEIVE SD_RECEIVE

static inline int poll(struct pollfd *pfd, int nfds, int timeout) { return WSAPoll (pfd, nfds, timeout); }

#elif defined(__unix__) || defined(__linux__)

typedef int socket_t;
typedef ssize_t msg_size_t;

#define INVALID_SOCKET (socket_t)(-1)
#define SOCKET_ERROR (-1)
#define SOCKET_SEND SHUT_RDWR
#define SOCKET_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR errno

#else
#error Library is not supported on this platform

#endif


class Socket
{
protected:
	bool _closed;
	socket_t _socket;

public:
	explicit Socket(socket_t sock);
	int close();
	bool set_blocking(bool blocking);
};

__INTERNAL_END__


#endif // WASP_CORE_TCP_SOCKET_H
