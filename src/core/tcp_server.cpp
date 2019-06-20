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
 * tcp/ip server implementation.
 */

#include "tcp_server.h"


leaf::internal::TcpServer::TcpServer(const char* host, uint16_t port, leaf::internal::tcpHandler handler)
{
	this->host = std::string(host);
	this->port = port;
	this->handler = std::move(handler);
}

leaf::internal::TcpServer::~TcpServer()
{
	for (::std::thread& t : this->pool)
	{
		if (t.joinable())
		{
			t.join();
		}
	}
}

void leaf::internal::TcpServer::listenAndServe()
{
	WSA_STARTUP();
	this->startListener();
	WSA_CLEANUP();
}

void leaf::internal::TcpServer::startListener()
{
	SOCKET sock;
	sockaddr_in addr = {};
	socklen_t sa_size = sizeof(sockaddr_in);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->port);
	inet_pton(AF_INET, this->host.c_str(), &(addr.sin_addr));
	if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
	{
		// TODO: log error
		WSA_CLEANUP();
		return;
	}

	if (bind(sock, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		// TODO: log error
		TcpServer::cleanUp(sock);
		return;
	}

	if (listen(sock, SOMAXCONN) == SOCKET_ERROR)
	{
		// TODO: log error
		return;
	}

	bool listening = true;
	while (listening)
	{
		try
		{
			SOCKET connection = accept(sock, (sockaddr*)&addr, &sa_size);
			if (connection != INVALID_SOCKET)
			{
				::std::thread newThread(&TcpServer::processRequest, this, connection);
				this->pool.push_back(std::move(newThread));
				// TODO: add thread to pool
			}
			else
			{
				// TODO: log error
			}
		}
		catch (const std::exception& exc)
		{
			// TODO: log error
			listening = false;
		}
		catch (const char* exc)
		{
			// TODO: log error
			listening = false;
		}
		catch (...)
		{
			// TODO: log error
			listening = false;
		}
	}
}

void leaf::internal::TcpServer::processRequest(const SOCKET& connection)
{
	std::string data = TcpServer::recvAll(connection);

	const std::string resp = this->handler(data);

	TcpServer::sendResponse(resp.c_str(), connection);

	TcpServer::closeSocket(connection, SOCK_SEND);
	TcpServer::closeSocket(connection, SOCK_RECEIVE);

	TcpServer::cleanUp(connection);
}

std::string leaf::internal::TcpServer::recvAll(const SOCKET& connection)
{
	char buffer[MAX_BUFF_SIZE];
	MSG_SIZE msgSize;
	unsigned long size = 0;
	std::string data;
	do
	{
		msgSize = recv(connection, buffer, MAX_BUFF_SIZE, 0);
		if (msgSize > 0)
		{
			data += std::string(buffer);
			size += msgSize;
		//	data = data.substr(0, size);
		}
		else if (msgSize < 0)
		{
			// TODO: log error
		}
	}
	while (msgSize >= MAX_BUFF_SIZE);

	return data;
}

void leaf::internal::TcpServer::sendResponse(const char* data, const SOCKET& connection)
{
	if (send(connection, data, ::strlen(data), 0) == SOCKET_ERROR)
	{
		// TODO: log error
		TcpServer::cleanUp(connection);
	}
}

void leaf::internal::TcpServer::closeSocket(const SOCKET& connection, const int& type)
{
	if (shutdown(connection, type) == SOCKET_ERROR)
	{
		// TODO: log error
		TcpServer::cleanUp(connection);
	}
}

void leaf::internal::TcpServer::cleanUp(const SOCKET& connection)
{
	if (CLOSE_SOCK(connection) == SOCKET_ERROR)
	{
		// TODO: log error
	}
	WSA_CLEANUP();
}
