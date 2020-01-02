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
 * An implementation of server_socket.h.
 */

#include "./server_socket.h"


__CORE_INTERNAL_BEGIN__

ServerSocket::ServerSocket() : Socket(-1), _use_ipv6(false)
{
}

socket_t ServerSocket::create_ipv4(const char* host, uint16_t port)
{
	this->_ipv4_socket.sin_family = AF_INET;
	this->_ipv4_socket.sin_port = ::htons(port);
	this->_ipv4_socket.sin_addr.s_addr = ::inet_addr(host);
	::inet_pton(AF_INET, host, (void*) &this->_ipv4_socket.sin_addr.s_addr);
	return ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
}

socket_t ServerSocket::create_ipv6(const char* host, uint16_t port)
{
	this->_ipv6_socket.sin6_family = AF_INET6;
	this->_ipv6_socket.sin6_port = ::htons(port);
	::inet_pton(AF_INET6, host, (void*) &this->_ipv6_socket.sin6_addr.s6_addr);
	return ::socket(PF_INET6, SOCK_STREAM, IPPROTO_TCP);
}

socket_t ServerSocket::create(const char* host, uint16_t port, bool use_ipv6)
{
	this->_use_ipv6 = use_ipv6;
	if (this->_use_ipv6)
	{
		this->_socket = this->create_ipv6(host, port);
	}
	else
	{
		this->_socket = this->create_ipv4(host, port);
	}

	return this->_socket;
}

int ServerSocket::bind()
{
	if (this->_use_ipv6)
	{
		return ::bind(this->_socket, (sockaddr*) &this->_ipv6_socket, sizeof(this->_ipv6_socket));
	}

	return ::bind(this->_socket, (sockaddr*) &this->_ipv4_socket, sizeof(this->_ipv4_socket));
}

int ServerSocket::listen()
{
	return ::listen(this->_socket, SOMAXCONN);
}

socket_t ServerSocket::accept()
{
	socket_t conn;
	if (this->_use_ipv6)
	{
		socklen_t conn_len = sizeof(this->_ipv6_socket);
		conn = ::accept(this->_socket, (sockaddr*) &this->_ipv6_socket, &conn_len);
	}
	else
	{
		socklen_t conn_len = sizeof(this->_ipv4_socket);
		conn = ::accept(this->_socket, (sockaddr*) &this->_ipv4_socket, &conn_len);
	}

	if (conn == INVALID_SOCKET)
	{
		throw SocketError("Invalid socket connection", _ERROR_DETAILS_);
	}

	return conn;
}

__CORE_INTERNAL_END__
