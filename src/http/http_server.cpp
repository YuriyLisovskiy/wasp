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
 * sockets/ip server implementation.
 * TODO: write docs
 */

#include "http_server.h"


__INTERNAL_BEGIN__

// Private methods.
int HttpServer::_init()
{
	if (this->_serverSocket.create(this->_host, this->_port) == INVALID_SOCKET)
	{
		this->_logger->trace(
			"Failed to initialize server at port " + std::to_string(this->_port),
			_ERROR_DETAILS_
		);
		HttpServer::_wsaCleanUp();
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
		HttpServer::_wsaCleanUp();
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

void HttpServer::_cleanUp(const socket_t& client)
{
	int ret;
#if defined(_WIN32) || defined(_WIN64)
	ret = ::closesocket(client);
#elif defined(__unix__) || defined(__linux__)
	ret = ::close(client);
#else
	ret = SOCKET_ERROR;
#endif
	if (ret == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
	}
	HttpServer::_wsaCleanUp();
}

HttpRequest HttpServer::_handleRequest(const socket_t& client)
{
	request_parser rp;
	std::string bodyBeginning;

	std::string headers_str = this->_readHeaders(client, bodyBeginning);

	rp.parse_headers(headers_str);
	Dict<std::string, std::string> headers = rp.get_headers();
	if (headers.contains("Content-Length"))
	{
		size_t bodyLength = strtol(headers.get("Content-Length").c_str(), nullptr, 10);
		std::string body;
		if (bodyLength == bodyBeginning.size())
		{
			body = bodyBeginning;
		}
		else
		{
			body = this->_readBody(client, bodyBeginning, bodyLength);
		}

		rp.parse_body(body, this->_mediaRoot);
	}

	return rp.build_request();
}

std::string HttpServer::_readBody(
	const wasp::internal::socket_t& client,
	const std::string& bodyBeginning,
	size_t bodyLength
)
{
	std::string data;
	if (bodyLength <= 0)
	{
		return data;
	}

	size_t size = bodyBeginning.size();
	if (size == bodyLength)
	{
		return bodyBeginning;
	}

	msg_size_t ret = 0;
	size_t buff_size = MAX_BUFF_SIZE < bodyLength ? MAX_BUFF_SIZE : bodyLength;
	char* buffer = (char*) calloc(buff_size, sizeof(char));
	while (size < bodyLength)
	{
	//	ret = read(client, buffer, buff_size);
		ret = ::recv(client, buffer, buff_size, 0);
		if (ret > 0)
		{
			data.append(buffer, ret);
			size += ret;
			if (size > this->_maxBodySize)
			{
				throw SuspiciousOperation("Request data is too big", _ERROR_DETAILS_);
			}
		}
		else if (ret == -1)
		{
			HttpServer::ReadResult status = HttpServer::_handleError(buffer, _ERROR_DETAILS_);
			if (status == HttpServer::ReadResult::Continue)
			{
				continue;
			}
			else if (status == HttpServer::ReadResult::Break)
			{
				break;
			}
		}
	}

	free(buffer);
	data = bodyBeginning + data;
	if (data.size() != bodyLength)
	{
		throw HttpError("Actual body size is not equal to header's value", _ERROR_DETAILS_);
	}

	return data;
}

std::string HttpServer::_readHeaders(const wasp::internal::socket_t& client, std::string& bodyBeginning)
{
	msg_size_t ret = 0;
	unsigned long size = 0;
	std::string data;
	size_t headersDelimiterPos = std::string::npos;
	std::string delimiter = "\r\n\r\n";

	char* buffer = (char*) calloc(MAX_BUFF_SIZE, sizeof(char));
	do
	{
		ret = ::recv(client, buffer, MAX_BUFF_SIZE, 0);
		if (ret > 0)
		{
			data.append(buffer, ret);
			size += ret;
		//	if (size > this->_maxRequestSize)
		//	{
		//		throw SuspiciousOperation("Request data is too big", _ERROR_DETAILS_);
		//	}
			// TODO: check headers size
		}
		else if (ret == -1)
		{
			HttpServer::ReadResult status = HttpServer::_handleError(buffer, _ERROR_DETAILS_);
			if (status == HttpServer::ReadResult::Continue)
			{
				continue;
			}
			else if (status == HttpServer::ReadResult::Break)
			{
				break;
			}
		}
		headersDelimiterPos = data.find(delimiter);
	}
	while (headersDelimiterPos == std::string::npos);

	free(buffer);
	if (headersDelimiterPos == std::string::npos)
	{
		throw HttpError("Invalid http request has been received", _ERROR_DETAILS_);
	}

	headersDelimiterPos += delimiter.size();
	bodyBeginning = data.substr(headersDelimiterPos);
	return data.substr(0, headersDelimiterPos);
}

void HttpServer::_startListener()
{
	bool listening = true;
	while (listening)
	{
		try
		{
			socket_t connection = this->_serverSocket.accept();

			if (connection != INVALID_SOCKET)
			{
			//	HttpServer::_setSocketBlocking(connection, false);

				std::thread newThread(&HttpServer::_threadFunc, this, connection);
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

void HttpServer::_serveConnection(const socket_t& client)
{
	HttpResponseBase* errorResponse = nullptr;
	try
	{
		// TODO: remove when release ------------------------:
		wasp::dt::Measure<std::chrono::milliseconds> measure;
		measure.start();
		// TODO: remove when release ------------------------^

		HttpRequest request = this->_handleRequest(client);
		this->_httpHandler(request, client);

		// TODO: remove when release -------------------------------------------------------------:
		measure.end();
		std::cout << '\n' << request.method() + " request took " << measure.elapsed() << " ms\n";
		// TODO: remove when release -------------------------------------------------------------^
	}
	catch (const SuspiciousOperation& exc)
	{
		this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
		errorResponse = new HttpResponseBadRequest(
			"<p style=\"font-size: 24px;\" >Bad Request</p>"
		);
	}
	catch (const wasp::BaseException& exc)
	{
		this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
		errorResponse = new HttpResponseServerError(
			"<p style=\"font-size: 24px;\" >Internal Server Error</p>"
		);
	}
	catch (const std::exception& exc)
	{
		this->_logger->trace(exc.what(), _ERROR_DETAILS_);
		errorResponse = new HttpResponseServerError(
			"<p style=\"font-size: 24px;\" >Internal Server Error</p>"
		);
	}
	if (errorResponse != nullptr)
	{
		HttpServer::_send(errorResponse->serialize().c_str(), client);
		delete errorResponse;
	}
}

void HttpServer::_threadFunc(const socket_t& client)
{
	try
	{
		this->_serveConnection(client);
	}
	catch (const std::exception& exc)
	{
		this->_logger->trace(exc.what(), _ERROR_DETAILS_);
	}
	HttpServer::_cleanUp(client);
}

// Private static functions.
HttpServer::ReadResult HttpServer::_handleError(
	char* buffer, int line, const char *function, const char *file
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
			throw HttpError("Critical error: " + std::to_string(errno), line, function, file);
		case EIO:
		case ENOBUFS:
		case ENOMEM:
			// Resource acquisition failure or device error.
			free(buffer);
			throw HttpError("Resource failure: " + std::to_string(errno), line, function, file);
		case EINTR:
			// TODO: Check for user interrupt flags.
		case ETIMEDOUT:
		case EAGAIN:
			// Temporary error.
			return HttpServer::ReadResult::Continue;
		case ECONNRESET:
		case ENOTCONN:
			// Connection broken.
			// Return the data we have available and exit
			// as if the connection was closed correctly.
			return HttpServer::ReadResult::Break;
		default:
			free(buffer);
			throw HttpError("Returned -1: " + std::to_string(errno), line, function, file);
	}
}

void HttpServer::_normalizeContext(HttpServer::Context& ctx)
{
	if (ctx.host == nullptr)
	{
		ctx.host = DEFAULT_HOST;
	}

	if (ctx.port == 0)
	{
		ctx.port = DEFAULT_PORT;
	}

	if (ctx.maxBodySize == 0)
	{
		ctx.maxBodySize = MAX_BODY_SIZE;
	}

	if (ctx.handler == nullptr)
	{
		throw wasp::HttpError("HttpServer::Context::handler can not be nullptr", _ERROR_DETAILS_);
	}

	if (ctx.logger == nullptr)
	{
		ctx.logger = Logger::get_instance();
	}

	str::rtrim(ctx.mediaRoot, '/');
	str::rtrim(ctx.mediaRoot, '\\');
}

void HttpServer::_send(const char* data, const socket_t& client)
{
	if (::send(client, data, std::strlen(data), 0) == SOCKET_ERROR)
	{
		throw HttpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

bool HttpServer::_setSocketBlocking(int _sock, bool blocking)
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

void HttpServer::_write(const char* data, size_t bytesToWrite, const socket_t& client)
{
	if (::write(client, data, bytesToWrite) == -1)
	{
		throw HttpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

void HttpServer::_wsaCleanUp()
{
#if defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}

// Public methods.
HttpServer::HttpServer(HttpServer::Context& ctx) : _finished(true)
{
	HttpServer::_normalizeContext(ctx);

	this->_host = ctx.host;
	this->_port = ctx.port;
	this->_logger = ctx.logger;

	this->_maxBodySize = ctx.maxBodySize;

	// Default schema, https will be implemented in future.
	this->_schema = "http";

	this->_httpHandler = ctx.handler;

	this->_mediaRoot = ctx.mediaRoot;
}

HttpServer::~HttpServer()
{
	this->finish();
}

void HttpServer::finish()
{
	if (this->_finished)
	{
		return;
	}

	if (this->_serverSocket.close() == SOCKET_ERROR)
	{
		this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
	}
	HttpServer::_wsaCleanUp();
	this->_finished = true;
}

void HttpServer::listenAndServe()
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

	std::cout << wasp::str::format(STARTUP_MESSAGE, this->_schema, this->_host, this->_port);
	std::cout.flush();

	this->_finished = false;

	this->_startListener();
	this->finish();
}

void HttpServer::send(HttpResponseBase* response, const socket_t& client)
{
	HttpServer::_send(response->serialize().c_str(), client);
}

void HttpServer::send(StreamingHttpResponse* response, const socket_t& client)
{
	std::string chunk;
	while (!(chunk = response->getChunk()).empty())
	{
		HttpServer::_write(chunk.c_str(), chunk.size(), client);
	}
	response->close();
}

__INTERNAL_END__
