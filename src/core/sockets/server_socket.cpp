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

ServerSocket::ServerSocket() : Socket(-1)
{
}

socket_t ServerSocket::create(const char* host, uint16_t port)
{
	this->_socketAddr.sin_family = AF_INET;
	this->_socketAddr.sin_port = ::htons(port);
	this->_socketAddr.sin_addr.s_addr = ::inet_addr(host);

	::memset(this->_socketAddr.sin_zero, '\0', sizeof this->_socketAddr.sin_zero);

	this->_socket = ::socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	return this->_socket;
}

int ServerSocket::bind()
{
	return ::bind(this->_socket, (sockaddr*)&this->_socketAddr, sizeof(this->_socketAddr));
}

int ServerSocket::listen()
{
	return ::listen(this->_socket, SOMAXCONN);
}

socket_t ServerSocket::accept()
{
	socklen_t connLen = sizeof(this->_socketAddr);
	socket_t conn = ::accept(this->_socket, (sockaddr*)&this->_socketAddr, &connLen);
	if (conn == INVALID_SOCKET)
	{
		throw SocketError("Invalid socket connection", _ERROR_DETAILS_);
	}

	return conn;
}

__CORE_INTERNAL_END__
