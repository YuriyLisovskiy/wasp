/**
 * middleware/cookies.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Sets cookies in http request object using cookie parser.
 *
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"
#include "../http/internal/cookie_parser.h"


__MIDDLEWARE_BEGIN__

// TESTME: CookieMiddleware
// TODO: docs for 'CookieMiddleware'
class CookieMiddleware final : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::CookieMiddleware";

	explicit CookieMiddleware(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	inline Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override
	{
		request->COOKIES = collections::Dictionary(
			http::internal::parse_cookie(request->headers.get("Cookie", ""), "")
		);
		return this->none();
	}
};

__MIDDLEWARE_END__
