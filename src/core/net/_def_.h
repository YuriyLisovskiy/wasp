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
 * _def_.h
 * Purpose: core/net module's definitions.
 */

#pragma once

#include "../_def_.h"


/// wasp::core::net
#define __NET_BEGIN__ __CORE_BEGIN__ namespace net {
#define __NET_END__ } __CORE_END__

/// wasp::core::net::internal
#define __NET_INTERNAL_BEGIN__ __NET_BEGIN__ namespace internal {
#define __NET_INTERNAL_END__ } __NET_END__


#if defined(_WIN32) || defined(_WIN64)

#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment (lib, "Ws2_32.lib")

typedef SOCKET socket_t;
typedef int msg_size_t;

#define WSA_LAST_ERR WSAGetLastError()
#define SOCKET_SEND SD_SEND
#define SOCKET_RECEIVE SD_RECEIVE

static inline int poll(struct pollfd *pfd, int nfds, int timeout) { return WSAPoll (pfd, nfds, timeout); }

#elif defined(__unix__) || defined(__linux__)

#include <unistd.h>
#include <arpa/inet.h>

__NET_INTERNAL_BEGIN__

typedef int socket_t;
typedef ssize_t msg_size_t;

__NET_INTERNAL_END__

#define INVALID_SOCKET (socket_t)(-1)
#define SOCKET_ERROR (-1)
#define SOCKET_SEND SHUT_RDWR
#define SOCKET_RECEIVE SHUT_RDWR
#define WSA_LAST_ERR errno

#else
#error Library is not supported on this platform

#endif
