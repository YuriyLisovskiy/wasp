/**
 * core/server.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Server libraries.
#include <xalwart.server/server.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"


__CORE_BEGIN__

typedef std::function<
	Result<std::shared_ptr<http::IHttpResponse>>(http::HttpRequest*, const int&)
> HttpHandlerFunc;

class DefaultServer : server::HTTPServer
{
public:
	DefaultServer(const server::Context& ctx, HttpHandlerFunc  handler);

private:
	HttpHandlerFunc _http_handler;

private:
	server::HandlerFunc _make_handler();

	static std::shared_ptr<http::IHttpResponse> _from_error(core::Error* err);

	std::shared_ptr<http::HttpRequest> _process_request(server::internal::request_parser* parser);

	core::Error _send(http::IHttpResponse* response, const int& client);
	core::Error _send(http::StreamingHttpResponse* response, const int& client);

	void _send_response(
		http::HttpRequest* request, http::IHttpResponse* response, const int& client, ILogger* logger
	);
	static void _log_request(
		const std::string& info, unsigned short status_code, ILogger* logger
	);
};

__CORE_END__
