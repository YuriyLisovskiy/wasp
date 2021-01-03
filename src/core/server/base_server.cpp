/**
 * core/server/base_server.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include "./server.h"

// C++ libraries.
#include <cstring>

// Framework libraries.
#include "./util.h"


__SERVER_BEGIN__

HTTPServer::HTTPServer(Context ctx, HandlerFunc handler)
	: ctx(std::move(ctx)), _handler(std::move(handler))
{
	this->ctx.normalize();
	this->_threadPool = std::make_shared<core::ThreadPool>(this->ctx.workers);
	if (!this->_handler)
	{
		this->_handler = [this](const int, parsers::request_parser*, core::Error*)
		{
			this->ctx.logger->print("request handler is not specified", core::Logger::Color::RED);
		};
	}
}

void HTTPServer::bind(const std::string& address, uint16_t port)
{
	this->_socket = util::create_socket(address, port, 5, this->ctx.logger.get());
	this->_socket->set_options();
}

void HTTPServer::listen(const std::string& message)
{
	this->_socket->listen();
	if (!message.empty())
	{
		this->ctx.logger->print(message);
	}

	this->_accept();
}

void HTTPServer::close()
{
	this->_threadPool->wait();
	util::close_socket(this->_socket, this->ctx.logger.get());
}

core::Error HTTPServer::send(int sock, const char* data)
{
	if (::send(sock, data, std::strlen(data), MSG_NOSIGNAL) < 0)
	{
		return core::Error(
			core::HttpError, "failed to send bytes to socket connection", _ERROR_DETAILS_
		);
	}

	return core::Error::none();
}

core::Error HTTPServer::write(int sock, const char* data, size_t n)
{
	if (::write(sock, data, n) < 0)
	{
		return core::Error(
			core::HttpError, "failed to send bytes to socket connection", _ERROR_DETAILS_
		);
	}

	return core::Error::none();
}

void HTTPServer::_accept()
{
//	sockaddr_in6 newSocketInfo{};
//	socklen_t newSocketInfoLength = sizeof(newSocketInfo);
	int new_sock;
	while (!this->_socket->is_closed())
	{
		while ((new_sock = ::accept(this->_socket->fd(), nullptr, nullptr)) < 0)
		{
			if (errno == EBADF || errno == EINVAL)
			{
				throw core::SocketError(
					errno, "'accept' call failed: " + std::to_string(errno), _ERROR_DETAILS_
				);
			}

			if (errno == EMFILE)
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(300));
				new_sock = -1;
				break;
			}

			throw core::SocketError(
				errno,
				"'accept' call failed while accepting a new connection: " + std::to_string(errno),
				_ERROR_DETAILS_
			);
		}

		if (!this->_socket->is_closed() && new_sock >= 0)
		{
			this->_handle(new_sock);
		}
	}
}

void HTTPServer::_handle(const int& sock)
{
	this->_threadPool->push([this, sock](){
		try
		{
			Measure measure;
			measure.start();
			parsers::request_parser rp;
			xw::string body_beginning;
			auto result = _read_headers(sock, body_beginning);
			if (result.err)
			{
				this->ctx.logger->trace("Method '_read_headers' returned an error", _ERROR_DETAILS_);
				this->_handler(sock, &rp, &result.err);
			}
			else
			{
				rp.parse_headers(result.value);
				if (rp.headers.find("Content-Length") != rp.headers.end())
				{
					size_t body_length = std::strtol(rp.headers["Content-Length"].c_str(), nullptr, 10);
					xw::string body;
					if (body_length == body_beginning.size())
					{
						body = body_beginning;
					}
					else
					{
						result = _read_body(sock, body_beginning, body_length);
						if (result.err)
						{
							this->ctx.logger->trace("Method '_read_body' returned an error", _ERROR_DETAILS_);
							this->_handler(sock, &rp, &result.err);
	//						return result.forward<std::shared_ptr<http::HttpRequest>>();
						}
						else
						{
							body = result.value;
						}
					}

					rp.parse_body(body, this->ctx.media_root);
				}

				this->_handler(sock, &rp, nullptr);
			}

			::close(sock);
			measure.end();
			this->ctx.logger->debug("Time elapsed: " + std::to_string(measure.elapsed()) + " milliseconds");
		}
		catch (const core::ParseError& exc)
		{
			this->ctx.logger->error(exc);
		}
	});
}

int HTTPServer::_error()
{
	switch (errno)
	{
		case EBADF:
		case EFAULT:
		case EINVAL:
		case ENXIO:
			// Fatal error.
			return errno - 100;
		case EIO:
		case ENOBUFS:
		case ENOMEM:
			// Resource acquisition failure or device error.
			return errno - 101;
		case EINTR:
			// TODO: Check for user interrupt flags.
		case ETIMEDOUT:
		case EAGAIN:
			// Temporary error.
			return read_result_enum::rr_continue;
		case ECONNRESET:
		case ENOTCONN:
			// Connection broken.
			// Return the data we have available and exit
			// as if the connection was closed correctly.
			return read_result_enum::rr_break;
		default:
			return errno;
	}
}

core::Result<xw::string> HTTPServer::_read_headers(
	size_t sock, xw::string& body_beginning
)
{
	size_t size = 0;
	xw::string data;
	size_t headers_delimiter_pos = xw::string::npos;
	xw::string delimiter = "\r\n\r\n";

	char buffer[MAX_BUFF_SIZE];
	long long message_len;
	do
	{
		message_len = recv(sock, buffer, MAX_BUFF_SIZE, 0);
		buffer[message_len] = '\0';
		if (message_len > 0)
		{
			data.append(buffer);
			size += message_len;

			// Maybe it is better to check each header value's size.
			if (size > MAX_HEADERS_SIZE)
			{
				this->ctx.logger->trace("Request size is greater than expected", _ERROR_DETAILS_);
				return core::raise<core::EntityTooLargeError, xw::string>(
					"Request data is too big", _ERROR_DETAILS_
				);
			}
		}
		else if (message_len == -1)
		{
			auto status = _error();
			if (status == read_result_enum::rr_continue)
			{
				continue;
			}
			else if (status == read_result_enum::rr_break)
			{
				break;
			}
			else
			{
				this->ctx.logger->trace(
					"An error occurred during a receiving of the request", _ERROR_DETAILS_
				);
				return core::raise<core::HttpError, xw::string>(
					"request finished with error code " + std::to_string(status), _ERROR_DETAILS_
				);
			}
		}

		headers_delimiter_pos = data.find(delimiter);
	}
	while (headers_delimiter_pos == xw::string::npos);

	if (headers_delimiter_pos == xw::string::npos)
	{
		this->ctx.logger->trace("Received an incorrect request", _ERROR_DETAILS_);
		return core::raise<core::HttpError, xw::string>(
			"invalid http request has been received", _ERROR_DETAILS_
		);
	}

	headers_delimiter_pos += delimiter.size();
	body_beginning = data.substr(headers_delimiter_pos);
	return core::Result(data.substr(0, headers_delimiter_pos));
}

core::Result<xw::string> HTTPServer::_read_body(
	size_t sock, const xw::string& body_beginning, size_t body_length
)
{
	xw::string data;
	if (body_length <= 0)
	{
		return core::Result(data);
	}

	size_t size = body_beginning.size();
	if (size == body_length)
	{
		return core::Result(body_beginning);
	}

	long long message_len;
	char buffer[MAX_BUFF_SIZE];
	while (size < body_length)
	{
		message_len = recv(sock, buffer, MAX_BUFF_SIZE, 0);
		buffer[message_len] = '\0';
		if (message_len > 0)
		{
			data.append(buffer);
			size += message_len;
			if (size > this->ctx.max_body_size)
			{
				this->ctx.logger->trace("Body size is greater than expected", _ERROR_DETAILS_);
				return core::raise<core::EntityTooLargeError, xw::string>(
					"Request body is too big", _ERROR_DETAILS_
				);
			}
		}
		else if (message_len == -1)
		{
			auto status = _error();
			if (status == read_result_enum::rr_continue)
			{
				continue;
			}
			else if (status == read_result_enum::rr_break)
			{
				break;
			}
			else
			{
				this->ctx.logger->trace(
					"An error occurred during a receiving of the request", _ERROR_DETAILS_
				);
				return core::raise<core::HttpError, xw::string>(
					"request finished with error code " + std::to_string(status), _ERROR_DETAILS_
				);
			}
		}
	}

	data = body_beginning + data;
	if (data.size() != body_length)
	{
		this->ctx.logger->trace("Received an incorrect request", _ERROR_DETAILS_);
		return core::raise<core::HttpError, xw::string>(
			"actual body size is not equal to header's value", _ERROR_DETAILS_
		);
	}

	return core::Result(data);
}

__SERVER_END__
