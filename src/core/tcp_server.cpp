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


__INTERNAL_BEGIN__

TcpServer::TcpServer(TcpServer::Context ctx)
{
	if (ctx.host == nullptr)
	{
		ctx.host = "127.0.0.1";
	}

	if (ctx.port == 0)
	{
		ctx.port = 8000;
	}

	this->_host = ctx.host;
	this->_port = ctx.port;

	if (ctx.handler == nullptr)
	{
		throw std::invalid_argument("Context::handler can not be nullptr");
	}

	this->_handler = ctx.handler;

	if (ctx.logger == nullptr)
	{
		ctx.logger = Logger::getInstance();
	}

	this->_logger = ctx.logger;

	this->_socketAddr = {};
	this->_socket = {};
}

TcpServer::~TcpServer()
{
	TcpServer::cleanUp(this->_socket);
}

int TcpServer::init()
{
	this->_socketAddr.sin_family = AF_INET;
	this->_socketAddr.sin_port = htons(this->_port);
	inet_pton(AF_INET, this->_host, &(this->_socketAddr.sin_addr));

	this->_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->_socket == INVALID_SOCKET)
	{
		this->_logger->trace(
			"Failed to initialize server at port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			__FILE__, __FUNCTION__, __LINE__
		);
		TcpServer::wsaCleanUp();
		return INVALID_SOCKET;
	}

	if (bind(this->_socket, (sockaddr*)&this->_socketAddr, sizeof(this->_socketAddr)) == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to bind socket to port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			__FILE__, __FUNCTION__, __LINE__
		);
		TcpServer::cleanUp(this->_socket);
		return SOCKET_ERROR;
	}

	if (listen(this->_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to listen at port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			__FILE__, __FUNCTION__, __LINE__
		);
		return SOCKET_ERROR;
	}

	return 0;
}

void TcpServer::listenAndServe()
{
	if (this->init() != 0)
	{
		return;
	}

#if defined(_WIN32) || defined(_WIN64)
	int status;
	WSADATA wsaData;
	status = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (status != 0)
	{
		this->_logger->error("WSAStartup() failed with error #" + std::to_string(status));
		return;
	}
#endif

	this->startListener();
	TcpServer::wsaCleanUp();
}

void TcpServer::startListener()
{
	bool listening = true;
	while (listening)
	{
		try
		{
			socklen_t connectionLen = sizeof(this->_socketAddr);
			socket_t connection = accept(this->_socket, (sockaddr*)&this->_socketAddr, &connectionLen);
			if (connection != INVALID_SOCKET)
			{
				std::thread newThread(&TcpServer::serveConnection, this, connection);
				newThread.detach();
			}
			else
			{
				this->_logger->trace("Invalid socket connection", __FILE__, __FUNCTION__, __LINE__);
			}
		}
		catch (const std::exception& exc)
		{
			this->_logger->trace(exc.what(), __FILE__, __FUNCTION__, __LINE__);
			listening = false;
		}
		catch (const char* exc)
		{
			this->_logger->trace(exc, __FILE__, __FUNCTION__, __LINE__);
			listening = false;
		}
		catch (...)
		{
			this->_logger->trace(
				"Error occurred while listening for socket connection", __FILE__, __FUNCTION__, __LINE__
			);
			listening = false;
		}
	}
}

void TcpServer::serveConnection(const socket_t& connection)
{
	std::string data = TcpServer::recvAll(connection);

	const std::string resp = this->_handler(data.c_str());

	TcpServer::sendResponse(resp.c_str(), connection);

	TcpServer::closeConnection(connection, SOCKET_SEND);
	TcpServer::closeConnection(connection, SOCKET_RECEIVE);

	TcpServer::cleanUp(connection);
}

// TODO: bug, second, third, etc. POST requests from browser (Google Chrome, at least)
//  with 'multipart/form-data' content type are read without request body
std::string TcpServer::recvAll(const socket_t& connection)
{
	msg_size_t msgSize = 0;
	unsigned long size = 0;
	std::string data;
	char* buffer = (char*) calloc(MAX_BUFF_SIZE, sizeof(char));

	do
	{
		msgSize = recv(connection, buffer, MAX_BUFF_SIZE, 0);
		if (msgSize > 0)
		{
			data.append(buffer, msgSize);
			size += msgSize;
		}
		else if (msgSize < 0)
		{
			this->_logger->trace("Received message size is less than zero", __FILE__, __FUNCTION__, __LINE__);
		}
	}
	while (msgSize == MAX_BUFF_SIZE);

	free(buffer);
	return data;
}

void TcpServer::sendResponse(const char* data, const socket_t& connection)
{
	if (send(connection, data, ::strlen(data), 0) == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to send bytes to socket connection", __FILE__, __FUNCTION__, __LINE__);
		this->cleanUp(connection);
	}
}

void TcpServer::closeConnection(const socket_t& connection, const int& type)
{
	if (shutdown(connection, type) == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to shut down socket connection", __FILE__, __FUNCTION__, __LINE__);
		this->cleanUp(connection);
	}
}

int TcpServer::closeSocket(const socket_t& socket)
{
#if defined(_WIN32) || defined(_WIN64)
	return closesocket(socket);
#elif defined(__unix__) || defined(__linux__)
	return close(socket);
#endif
}

void TcpServer::wsaCleanUp()
{
#if defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}

void TcpServer::cleanUp(const socket_t& socket)
{
	if (TcpServer::closeSocket(socket) == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to close socket connection", __FILE__, __FUNCTION__, __LINE__);
	}
	TcpServer::wsaCleanUp();
}

__INTERNAL_END__
