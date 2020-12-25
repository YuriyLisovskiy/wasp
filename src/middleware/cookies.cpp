/**
 * middleware/cookies.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./cookies.h"

// Framework libraries.
#include "../core/parsers/cookie_parser.h"


__MIDDLEWARE_BEGIN__

const std::string CookieMiddleware::FULL_NAME = "xw::middleware::CookieMiddleware";

CookieMiddleware::CookieMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

http::Result<std::shared_ptr<http::IHttpResponse>> CookieMiddleware::process_request(
	http::HttpRequest* request
)
{
	core::internal::cookie_parser cp;
	cp.parse(request->headers.get("Cookie", ""));
	request->COOKIES = collections::Dict(cp.result);
	return this->none();
}

__MIDDLEWARE_END__
