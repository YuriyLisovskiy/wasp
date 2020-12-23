/**
 * core/net/socket.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: tcp/ip socket's wrapper.
 */

#pragma once

#if defined(_WIN32) || defined(_WIN64)

// C++ libraries.
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
