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

#include "tcp_server.h"


__INTERNAL_BEGIN__

TcpServer::TcpServer(TcpServer::Context ctx)
{
	if (ctx.host == nullptr)
	{
		ctx.host = DEFAULT_HOST;
	}

	if (ctx.port == 0)
	{
		ctx.port = DEFAULT_PORT;
	}

	this->_host = ctx.host;
	this->_port = ctx.port;

	if (ctx.handler == nullptr)
	{
		throw std::invalid_argument("Context::handler can not be null");
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
	this->_socketAddr.sin_addr.s_addr = inet_addr(this->_host);

	memset(this->_socketAddr.sin_zero, '\0', sizeof this->_socketAddr.sin_zero);

	this->_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (this->_socket == INVALID_SOCKET)
	{
		this->_logger->trace(
			"Failed to initialize server at port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			_ERROR_DETAILS_
		);
		TcpServer::wsaCleanUp();
		return INVALID_SOCKET;
	}

	if (bind(this->_socket, (sockaddr*)&this->_socketAddr, sizeof(this->_socketAddr)) == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to bind socket to port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			_ERROR_DETAILS_
		);
		TcpServer::cleanUp(this->_socket);
		return SOCKET_ERROR;
	}

	if (listen(this->_socket, SOMAXCONN) == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to listen at port " + std::to_string(ntohs(this->_socketAddr.sin_port)),
			_ERROR_DETAILS_
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
				this->_logger->trace("Invalid socket connection", _ERROR_DETAILS_);
			}
		}
		catch (const std::exception& exc)
		{
			this->_logger->trace(exc.what(), _ERROR_DETAILS_);
			listening = false;
		}
		catch (const char* exc)
		{
			this->_logger->trace(exc, _ERROR_DETAILS_);
			listening = false;
		}
		catch (...)
		{
			this->_logger->trace(
				"Error occurred while listening for socket connection", _ERROR_DETAILS_
			);
			listening = false;
		}
	}
}

void TcpServer::serveConnection(const socket_t& connection)
{
	try
	{
		std::string data = TcpServer::recvAll(connection);

		if (!data.empty())
		{
			this->_handler(data, connection);
		}
	}
	catch (const BaseException& exc)
	{
		this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
	}
	catch (const std::exception& exc)
	{
		this->_logger->trace(exc.what(), _ERROR_DETAILS_);
	}
	TcpServer::cleanUp(connection);
}

std::string TcpServer::recvAll(const socket_t& connection)
{
//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	msg_size_t ret = 0;
	unsigned long size = 0;
	std::string data;

	// TODO: browser can't connect while post request with multipart/form-data enctype (if non-blocking socket)
	fcntl(connection, F_SETFL, O_NONBLOCK);

	char* buffer = (char*) calloc(MAX_BUFF_SIZE, sizeof(char));
	do
	{
		// TODO: recv hangs after all request has been read (if blocking socket)
		ret = read(connection, buffer, MAX_BUFF_SIZE);
		if (ret > 0)
		{
			data.append(buffer, ret);
			size += ret;
		}
		else if (ret == -1)
		{
			switch (errno)
			{
				case EBADF:
				case EFAULT:
				case EINVAL:
				case ENXIO:
					// Fatal error.
					free(buffer);
					throw TcpError("Read: critical error: " + std::to_string(errno), _ERROR_DETAILS_);
				case EIO:
				case ENOBUFS:
				case ENOMEM:
					// Resource acquisition failure or device error.
					free(buffer);
					throw TcpError("Read: resource failure: " + std::to_string(errno), _ERROR_DETAILS_);
				case EINTR:
					// TODO: Check for user interrupt flags.
				case ETIMEDOUT:
				case EAGAIN:
					// Temporary error.
					continue;
				case ECONNRESET:
				case ENOTCONN:
					// Connection broken.
					// Return the data we have available and exit
					// as if the connection was closed correctly.
					ret = 0;
					break;
				default:
					free(buffer);
					throw TcpError("Read: returned -1: " + std::to_string(errno), _ERROR_DETAILS_);
			}
		}
	}
	while (ret > 0);

	free(buffer);

	if (data.size() != size)
	{
		throw TcpError("Invalid request data total size", _ERROR_DETAILS_);
	}
	return data;
}

void TcpServer::send(const char* data, const socket_t& connection)
{
	if (::send(connection, data, std::strlen(data), 0) == SOCKET_ERROR)
	{
		throw TcpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

void TcpServer::write(const char* data, size_t bytesToWrite, const socket_t& connection)
{
	if (::write(connection, data, bytesToWrite) == -1)
	{
		throw TcpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
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
		this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
	}
	TcpServer::wsaCleanUp();
}

__INTERNAL_END__
