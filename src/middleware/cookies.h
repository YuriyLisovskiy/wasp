/**
 * middleware/cookies.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: enables cookies in http request object.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./middleware_mixin.h"


__MIDDLEWARE_BEGIN__

class CookieMiddleware final: public MiddlewareMixin
{
public:
	static const std::string FULL_NAME;

	explicit CookieMiddleware(conf::Settings* settings);
	CookieMiddleware(nullptr_t) = delete;

	std::unique_ptr<http::IHttpResponse> process_request(
		http::HttpRequest* request
	) final;
};

__MIDDLEWARE_END__
