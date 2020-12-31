/**
 * core/server/server.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base_server.h"
#include "../../http/request.h"
#include "../../http/response.h"
#include "../../conf/settings.h"


__SERVER_BEGIN__

typedef std::function<
	core::Result<std::shared_ptr<http::IHttpResponse>>(http::HttpRequest*, const int&)
> HttpHandlerFunc;

class DefaultServer : public HTTPServer
{
public:
	explicit DefaultServer(
		const Context& ctx,
		HttpHandlerFunc handler,
		const conf::Settings* settings
	);

protected:
	const conf::Settings* settings;

private:
	HttpHandlerFunc _http_handler;

private:
	HandlerFunc _make_handler();

	static std::shared_ptr<http::IHttpResponse> _from_error(core::Error* err);

	std::shared_ptr<http::HttpRequest> _process_request(parsers::request_parser* parser);

	core::Error _send(http::IHttpResponse* response, const int& client);
	core::Error _send(http::StreamingHttpResponse* response, const int& client);

	void _send_response(
		http::HttpRequest* request, http::IHttpResponse* response, const int& client, core::ILogger* logger
	);
	static void _log_request(
		const std::string& info, unsigned short status_code, core::ILogger* logger
	);
};

__SERVER_END__
