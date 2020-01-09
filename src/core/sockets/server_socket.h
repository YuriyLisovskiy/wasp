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
 * server_socket.h
 * Purpose: improved and adapted tcp/ip socket for server.
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./socket.h"
#include "../exceptions.h"


__CORE_INTERNAL_BEGIN__

class ServerSocket : public Socket
{
private:
	bool _use_ipv6;
	sockaddr_in _ipv4_socket{};
	sockaddr_in6 _ipv6_socket{};

public:
	ServerSocket();

	socket_t create(const char* host, uint16_t port, bool use_ipv6 = false);
	int bind();
	int listen();
	socket_t accept();

private:
	socket_t create_ipv4(const char* host, uint16_t port);
	socket_t create_ipv6(const char* host, uint16_t port);
};

__CORE_INTERNAL_END__
