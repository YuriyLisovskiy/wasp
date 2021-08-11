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

// TESTME: Cookie
// TODO: docs for 'Cookie'
class Cookie final : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::Cookie";

	explicit Cookie(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	inline http::Response::Result process_request(http::Request* request) override
	{
		request->COOKIES = collections::Dictionary(
			http::internal::parse_cookie(request->headers.get("Cookie", ""), "")
		);
		return {};
	}
};

__MIDDLEWARE_END__
