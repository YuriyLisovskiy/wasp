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
 * TODO: write docs
 */

#include "http_server.h"


__INTERNAL_BEGIN__

HttpServer::HttpServer(HttpServer::Context& ctx)
{
	HttpServer::normalizeContext(ctx);

	this->_host = ctx.host;
	this->_port = ctx.port;
	this->_logger = ctx.logger;

	// TODO: default schema, https will be implemented in future.
	this->_schema = "http";

	TcpServer::Context tcpContext{};
	tcpContext.host = ctx.host;
	tcpContext.port = ctx.port;
	tcpContext.handler = std::bind(&HttpServer::_tcpHandler, this, std::placeholders::_1);
	tcpContext.logger = ctx.logger;

	this->_httpHandler = ctx.handler;
	this->_tcpServer = new TcpServer(tcpContext);
}

HttpServer::~HttpServer()
{
	this->finish();
}

std::string HttpServer::_tcpHandler(const std::string& data)
{
	try
	{
		// TODO: remove when release ------------------------:
		wasp::dt::Measure<std::chrono::milliseconds> measure;
		measure.start();
		// TODO: remove when release ------------------------^

		HttpRequest request = HttpRequestParser().parse(data);
		HttpResponseBase* response = this->_httpHandler(request);

		// WARNING! Probably not working for large responses.
		std::string result = response->serialize();
		delete response;

		// TODO: remove when release -------------------------------------------------------------:
		measure.end();
		std::cout << '\n' << request.method() << " request took " << measure.elapsed() << " ms\n";
		// TODO: remove when release -------------------------------------------------------------^

		return result;
	}
	catch (const wasp::BaseException& exc)
	{
		this->_logger->trace(exc.what(), exc.line(), exc.function(), exc.file());

		// TODO: send internal server error
		return HttpResponseServerError("<p style=\"font-size: 24px;\" >Internal Server Error</p>").serialize();
	}
}

void HttpServer::listenAndServe()
{
	std::cout << wasp::str::format(STARTUP_MESSAGE, this->_schema, this->_host, this->_port);
	std::cout.flush();
	this->_tcpServer->listenAndServe();
}

void HttpServer::finish()
{
	delete this->_tcpServer;
}

void HttpServer::normalizeContext(HttpServer::Context& ctx)
{
	if (ctx.host == nullptr)
	{
		ctx.host = DEFAULT_HOST;
	}

	if (ctx.port == 0)
	{
		ctx.port = DEFAULT_PORT;
	}

	if (ctx.handler == nullptr)
	{
		throw wasp::HttpError("HttpServer::Context::handler can not be nullptr", _ERROR_DETAILS_);
	}

	if (ctx.logger == nullptr)
	{
		ctx.logger = Logger::getInstance();
	}
}

__INTERNAL_END__
