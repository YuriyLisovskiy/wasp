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
 * An implementation of http_server.h.
 */

#include "http_server.h"


__INTERNAL_BEGIN__

// Public methods.
HttpServer::HttpServer(HttpServer::context& ctx) : _finished(true)
{
	HttpServer::_normalize_context(ctx);

	this->_host = ctx.host;
	this->_port = ctx.port;
	this->_logger = ctx.logger;

	this->_max_body_size = ctx.max_body_size;
	this->_threads_count = ctx.threads_count;

	// Default schema, https will be implemented in future.
	this->_schema = "http";

	this->_http_handler = ctx.handler;

	this->_media_root = ctx.media_root;

	this->_thread_pool = new ThreadPool(ctx.threads_count);
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

	if (this->_server_socket.close() == SOCKET_ERROR)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
		}
	}

	HttpServer::_wsa_clean_up();

	delete this->_thread_pool;
	this->_finished = true;
}

void HttpServer::listen_and_serve()
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
		if (this->_logger != nullptr)
		{
			this->_logger->error("WSAStartup() failed with error #" + std::to_string(status));
		}
		return;
	}
#endif

	std::cout << wasp::str::format(STARTUP_MESSAGE, this->_schema, this->_host, this->_port);
	std::cout.flush();

	this->_finished = false;

	this->_start_listener();
}

void HttpServer::send(HttpResponseBase* response, const socket_t& client)
{
	HttpServer::_send(response->serialize().c_str(), client);
}

void HttpServer::send(StreamingHttpResponse* response, const socket_t& client)
{
	std::string chunk;
	while (!(chunk = response->get_chunk()).empty())
	{
		HttpServer::_write(chunk.c_str(), chunk.size(), client);
	}
	response->close();
}

// Private methods.
int HttpServer::_init()
{
	int init_status = this->_create();
	if (init_status == INVALID_SOCKET)
	{
		return init_status;
	}

	init_status = this->_bind();
	if (init_status == SOCKET_ERROR)
	{
		return init_status;
	}

	init_status = this->_listen();
	if (init_status == SOCKET_ERROR)
	{
		return init_status;
	}

	return 0;
}

int HttpServer::_create()
{
	if (this->_server_socket.create(this->_host, this->_port) == INVALID_SOCKET)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(
				"Failed to initialize server at port " + std::to_string(this->_port),
				_ERROR_DETAILS_
			);
		}

		HttpServer::_wsa_clean_up();
		return INVALID_SOCKET;
	}

	return 0;
}

int HttpServer::_bind()
{
	if (this->_server_socket.bind() == SOCKET_ERROR)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(
				"Failed to bind socket to port " + std::to_string(this->_port),
				_ERROR_DETAILS_
			);
		}

		if (this->_server_socket.close() == SOCKET_ERROR)
		{
			if (this->_logger != nullptr)
			{
				this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
			}
		}

		HttpServer::_wsa_clean_up();
		return SOCKET_ERROR;
	}

	return 0;
}

int HttpServer::_listen()
{
	if (this->_server_socket.listen() == SOCKET_ERROR)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(
				"Failed to listen at port " + std::to_string(this->_port),
				_ERROR_DETAILS_
			);
		}

		return SOCKET_ERROR;
	}

	return 0;
}

void HttpServer::_clean_up(const socket_t& client)
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
		if (this->_logger != nullptr)
		{
			this->_logger->trace("Failed to close socket connection", _ERROR_DETAILS_);
		}
	}
	HttpServer::_wsa_clean_up();
}

HttpRequest* HttpServer::_handle_request(const socket_t& client)
{
	request_parser rp;
	std::string body_beginning;

	std::string headers_str = HttpServer::_read_headers(client, body_beginning);

	rp.parse_headers(headers_str);
	Dict<std::string, std::string> headers = rp.get_headers();
	if (headers.contains("Content-Length"))
	{
		size_t body_length = std::strtol(headers.get("Content-Length").c_str(), nullptr, 10);
		std::string body;
		if (body_length == body_beginning.size())
		{
			body = body_beginning;
		}
		else
		{
			body = this->_read_body(client, body_beginning, body_length);
		}

		rp.parse_body(body, this->_media_root);
	}

	return rp.build_request();
}

std::string HttpServer::_read_body(
	const socket_t& client, const std::string& body_beginning, size_t body_length
)
{
	std::string data;
	if (body_length <= 0)
	{
		return data;
	}

	size_t size = body_beginning.size();
	if (size == body_length)
	{
		return body_beginning;
	}

	msg_size_t ret = 0;
	size_t buff_size = MAX_BUFF_SIZE < body_length ? MAX_BUFF_SIZE : body_length;
	char* buffer = (char*) calloc(buff_size, sizeof(char));
	while (size < body_length)
	{
	//	ret = read(client, buffer, buff_size);
		ret = ::recv(client, buffer, buff_size, 0);
		if (ret > 0)
		{
			data.append(buffer, ret);
			size += ret;
			if (size > this->_max_body_size)
			{
				throw EntityTooLargeError("Request data is too big", _ERROR_DETAILS_);
			}
		}
		else if (ret == -1)
		{
			HttpServer::read_result_enum status = HttpServer::_handle_error(buffer, _ERROR_DETAILS_);
			if (status == HttpServer::read_result_enum::rr_continue)
			{
				continue;
			}
			else if (status == HttpServer::read_result_enum::rr_break)
			{
				break;
			}
		}
	}

	free(buffer);
	data = body_beginning + data;
	if (data.size() != body_length)
	{
		throw HttpError("Actual body size is not equal to header's value", _ERROR_DETAILS_);
	}

	return data;
}

std::string HttpServer::_read_headers(const socket_t& client, std::string& body_beginning)
{
	msg_size_t ret = 0;
	unsigned long size = 0;
	std::string data;
	size_t headers_delimiter_pos = std::string::npos;
	std::string delimiter = "\r\n\r\n";

	char* buffer = (char*) calloc(MAX_BUFF_SIZE, sizeof(char));
	do
	{
		ret = ::recv(client, buffer, MAX_BUFF_SIZE, 0);
		if (ret > 0)
		{
			data.append(buffer, ret);
			size += ret;

			// Maybe it is better to check each header value's size.
			if (size > MAX_HEADERS_SIZE)
			{
				throw EntityTooLargeError("Request data is too big", _ERROR_DETAILS_);
			}
		}
		else if (ret == -1)
		{
			HttpServer::read_result_enum status = HttpServer::_handle_error(buffer, _ERROR_DETAILS_);
			if (status == HttpServer::read_result_enum::rr_continue)
			{
				continue;
			}
			else if (status == HttpServer::read_result_enum::rr_break)
			{
				break;
			}
		}

		headers_delimiter_pos = data.find(delimiter);
	}
	while (headers_delimiter_pos == std::string::npos);

	free(buffer);
	if (headers_delimiter_pos == std::string::npos)
	{
		throw HttpError("Invalid http request has been received", _ERROR_DETAILS_);
	}

	headers_delimiter_pos += delimiter.size();
	body_beginning = data.substr(headers_delimiter_pos);
	return data.substr(0, headers_delimiter_pos);
}

void HttpServer::_start_listener()
{
	bool listening = true;
	while (listening)
	{
		try
		{
			socket_t connection = this->_server_socket.accept();
			if (connection != INVALID_SOCKET)
			{
				this->_thread_pool->push(
					[this, connection] { this->_thread_func(connection); }
				);
			}
			else
			{
				if (this->_logger != nullptr)
				{
					this->_logger->trace("Invalid socket connection", _ERROR_DETAILS_);
				}
			}
		}
		catch (const wasp::InterruptException& exc)
		{
			throw InterruptException(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const std::exception& exc)
		{
			if (this->_logger != nullptr)
			{
				this->_logger->trace(exc.what(), _ERROR_DETAILS_);
			}

			listening = false;
		}
		catch (const char* exc)
		{
			if (this->_logger != nullptr)
			{
				this->_logger->trace(exc, _ERROR_DETAILS_);
			}

			listening = false;
		}
		catch (...)
		{
			if (this->_logger != nullptr)
			{
				this->_logger->trace(
					"Error occurred while listening for socket connection", _ERROR_DETAILS_
				);
			}

			listening = false;
		}
	}
}

void HttpServer::_serve_connection(const socket_t& client)
{
	HttpResponseBase* error_response = nullptr;
	try
	{
		// TODO: remove when release ------------------------:
		wasp::dt::Measure<std::chrono::milliseconds> measure;
		measure.start();
		// TODO: remove when release ------------------------^

		HttpRequest* request = this->_handle_request(client);
		this->_http_handler(request, client);
		delete request;

		// TODO: remove when release -------------------------------------------------------------:
		measure.end();
		std::cout << '\n' << request->method() + " request took " << measure.elapsed() << " ms\n";
		// TODO: remove when release -------------------------------------------------------------^
	}
	catch (const SuspiciousOperation& exc)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
		}
		error_response = new HttpResponseBadRequest(
			"<p style=\"font-size: 24px;\" >Bad Request</p>"
		);
	}
	catch (const EntityTooLargeError& exc)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
		}
		error_response = new HttpResponseEntityTooLarge(
			"<p style=\"font-size: 24px;\" >Entity Too Large</p>"
		);
	}
	catch (const wasp::BaseException& exc)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());
		}

		error_response = new HttpResponseServerError(
			"<p style=\"font-size: 24px;\" >Internal Server Error</p>"
		);
	}
	catch (const std::exception& exc)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(exc.what(), _ERROR_DETAILS_);
		}

		error_response = new HttpResponseServerError(
			"<p style=\"font-size: 24px;\" >Internal Server Error</p>"
		);
	}
	if (error_response != nullptr)
	{
		HttpServer::_send(error_response->serialize().c_str(), client);
		delete error_response;
	}
}

void HttpServer::_thread_func(const socket_t& client)
{
	try
	{
		this->_serve_connection(client);
	}
	catch (const std::exception& exc)
	{
		if (this->_logger != nullptr)
		{
			this->_logger->trace(exc.what(), _ERROR_DETAILS_);
		}
	}

	HttpServer::_clean_up(client);
}

// Private static functions.
HttpServer::read_result_enum HttpServer::_handle_error(
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
			return HttpServer::read_result_enum::rr_continue;
		case ECONNRESET:
		case ENOTCONN:
			// Connection broken.
			// Return the data we have available and exit
			// as if the connection was closed correctly.
			return HttpServer::read_result_enum::rr_break;
		default:
			free(buffer);
			throw HttpError("Returned -1: " + std::to_string(errno), line, function, file);
	}
}

void HttpServer::_normalize_context(HttpServer::context& ctx)
{
	if (ctx.host == nullptr)
	{
		ctx.host = DEFAULT_HOST;
	}

	if (ctx.port == 0)
	{
		ctx.port = DEFAULT_PORT;
	}

	if (ctx.max_body_size == 0)
	{
		ctx.max_body_size = MAX_BODY_SIZE;
	}

	if (ctx.threads_count == 0)
	{
		ctx.threads_count = DEFAULT_THREADS_COUNT;
	}

	if (ctx.handler == nullptr)
	{
		throw wasp::HttpError("HttpServer::Context::handler can not be nullptr", _ERROR_DETAILS_);
	}

	str::rtrim(ctx.media_root, '/');
	str::rtrim(ctx.media_root, '\\');
}

void HttpServer::_send(const char* data, const socket_t& client)
{
	if (::send(client, data, std::strlen(data), 0) == SOCKET_ERROR)
	{
		throw HttpError("Failed to send bytes to socket connection", _ERROR_DETAILS_);
	}
}

bool HttpServer::_set_socket_blocking(int _sock, bool blocking)
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

void HttpServer::_wsa_clean_up()
{
#if defined(_WIN32) || defined(_WIN64)
	WSACleanup();
#endif
}

__INTERNAL_END__
