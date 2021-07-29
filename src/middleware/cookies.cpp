/**
 * middleware/cookies.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./cookies.h"

// Framework libraries.
#include "../http/parsers/cookie_parser.h"


__MIDDLEWARE_BEGIN__

const std::string CookieMiddleware::FULL_NAME = "xw::middleware::CookieMiddleware";

CookieMiddleware::CookieMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

Result<std::shared_ptr<http::IHttpResponse>> CookieMiddleware::process_request(
	http::HttpRequest* request
)
{
	http::internal::cookie_parser cp;
	cp.parse(request->headers.get("Cookie", ""));
	request->COOKIES = collections::Dictionary(cp.result);
	return this->none();
}

__MIDDLEWARE_END__
