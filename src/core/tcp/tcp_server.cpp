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
}

TcpServer::~TcpServer()
{
	if (this->_serverSocket.close() == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
	}
	TcpServer::WSACleanUp();
}

// Private methods.
int TcpServer::_init()
{
	if (this->_serverSocket.create(this->_host, this->_port) == INVALID_SOCKET)
	{
		this->_logger->trace(
			"Failed to initialize server at port " + std::to_string(this->_port),
			_ERROR_DETAILS_
		);
		TcpServer::WSACleanUp();
		return INVALID_SOCKET;
	}

	if (this->_serverSocket.bind() == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to bind socket to port " + std::to_string(this->_port),
			_ERROR_DETAILS_
		);
		if (this->_serverSocket.close() == SOCKET_ERROR)
		{
			this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
		}
		TcpServer::WSACleanUp();
		return SOCKET_ERROR;
	}

	if (this->_serverSocket.listen() == SOCKET_ERROR)
	{
		this->_logger->trace(
			"Failed to listen at port " + std::to_string(this->_port),
			_ERROR_DETAILS_
		);
		return SOCKET_ERROR;
	}

	return 0;
}

void TcpServer::_cleanUp(const socket_t& socket)
{
	int ret;
#if defined(_WIN32) || defined(_WIN64)
	ret = ::closesocket(socket);
#elif defined(__unix__) || defined(__linux__)
	ret = ::close(socket);
#else
	ret = SOCKET_ERROR;
#endif
	if (ret == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
	}
	TcpServer::WSACleanUp();
}

void TcpServer::_serveConnection(const socket_t& connection)
{
	try
	{
		std::string data = TcpServer::RecvAll(connection);

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
	TcpServer::_cleanUp(connection);
}

void TcpServer::_startListener()
{
	bool listening = true;
	while (listening)
	{
		try
		{
			socket_t connection = this->_serverSocket.accept();

			if (connection != INVALID_SOCKET)
			{
				// TODO: browser can't connect while post request with
				//  multipart/form-data enctype (if non-blocking socket)
				TcpServer::SetSocketBlocking(connection, false);

				std::thread newThread(&TcpServer::_serveConnection, this, connection);
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

// Private static functions.
TcpServer::ReadResult TcpServer::HandleError(
	char* buffer, int& status, int line, const char *function, const char *file
)
{
	switch (errno)
	{
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENXIO:
			// Fatal error.
			free(buffer);
			throw TcpError("Critical error: " + std::to_string(errno), line, function, file);
		case EIO:
		case ENOBUFS:
		case ENOMEM:
			// Resource acquisition failure or device error.
			free(buffer);
			throw TcpError("Resource failure: " + std::to_string(errno), line, function, file);
		case EINTR:
			// TODO: Check for user interrupt flags.
		case ETIMEDOUT:
		case EAGAIN:
			// Temporary error.
			return ReadResult::Continue;
		case ECONNRESET:
		case ENOTCONN:
			// Connection broken.
			// Return the data we have available and exit
			// as if the connection was closed correctly.
			status = 0;
			break;
		default:
			free(buffer);
			throw TcpError("Returned -1: " + std::to_string(errno), line, function, file);
	}
	return ReadResult::None;
}

std::string TcpServer::RecvAll(const socket_t& connection)
{
	msg_size_t ret = 0;
	int status = 0;
	unsigned long size = 0;
	std::string data;

	// Poll descriptor structure
	struct pollfd descriptor{};

	// от sock1 мы будем ожидать входящих данных
	descriptor.fd = connection;
	descriptor.events = POLLIN;

	char* buffer = (char*) calloc(MAX_BUFF_SIZE, sizeof(char));
	do
	{
		// Wait 20 ms
		status = poll(&descriptor, 1, SO_RCVTIMEO);
		if (status == -1)
		{
			TcpServer::HandleError(buffer, status, _ERROR_DETAILS_);
		}
		else if (status == 0)
		{
			// Timeout, skip
		}
		else
		{
			// Reset the descriptor.revents to reuse the structure
			if (descriptor.revents == POLLIN)
			{
				descriptor.revents = 0;
			}

			ret = read(connection, buffer, MAX_BUFF_SIZE);
			if (ret > 0)
			{
				data.append(buffer, ret);
				size += ret;
			}
			else if (ret == -1)
			{
				TcpServer::HandleError(buffer, status, _ERROR_DETAILS_);
				/*
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
						status = 0;
						break;
					default:
						free(buffer);
						throw TcpError("Read: returned -1: " + std::to_string(errno), _ERROR_DETAILS_);
				}
				*/
			}
		}
	}
	while (status > 0);

	free(buffer);

	if (data.size() != size)
	{
		throw TcpError("Invalid request data total size", _ERROR_DETAILS_);
	}
	return data;
}

bool TcpServer::SetSocketBlocking(int _sock, bool blocking)
{
	if (_sock < 0)
	{
		return false;
	}

#if defined(_WIN32) || defined(_WIN64)
	unsigned long mode = blocking ? 0 : 1;
	return ioctlsocket(_sock, FIONBIO, &mode) == 0;
#else
	int flags = fcntl(_sock, F_GETFL, 0);
	if (flags == -1)
	{
		return false;
	}

	flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
	return fcntl(_sock, F_SETFL, flags) == 0;
#endif
}

void TcpServer::WSACleanUp()
{
#if defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}

// Public methods.
void TcpServer::listenAndServe()
{
	if (this->_init() != 0)
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

	this->_startListener();
	TcpServer::WSACleanUp();
}

// Public static functions.
void TcpServer::Send(const char* data, const socket_t& connection)
{
	if (::send(connection, data, std::strlen(data), 0) == SOCKET_ERROR)
	{
		throw TcpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

void TcpServer::Write(const char* data, size_t bytesToWrite, const socket_t& connection)
{
	if (::write(connection, data, bytesToWrite) == -1)
	{
		throw TcpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

__INTERNAL_END__
