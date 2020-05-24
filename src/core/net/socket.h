/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/net/socket.h
 *
 * Purpose: tcp/ip socket's wrapper.
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)
#include <cstdint>
#endif

// Module definitions.
#include "./_def_.h"


__NET_INTERNAL_BEGIN__

class Socket final
{
private:
	bool _closed;
	socket_t _socket;
	bool _use_ipv6;
	sockaddr_in _ipv4_socket{};
	sockaddr_in6 _ipv6_socket{};

public:
	Socket();

	[[nodiscard]] int initialize() const;
	socket_t create(const char* host, uint16_t port, bool use_ipv6 = false);
	int bind();
	[[nodiscard]] int listen() const;
	[[nodiscard]] socket_t accept() const;
	[[nodiscard]] int close() const;
	[[nodiscard]] int set_reuse_addr() const;
	[[nodiscard]] int set_reuse_port() const;
	[[nodiscard]] bool set_blocking(bool blocking) const;

	static int close_socket(socket_t s);

private:
	socket_t create_ipv4(const char* host, uint16_t port);
	socket_t create_ipv6(const char* host, uint16_t port);
};

__NET_INTERNAL_END__
