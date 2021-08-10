/**
 * middleware/http.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Handle conditional GET operations. If the response has an ETag or
 * Last-Modified header and the request has If-None-Match or If-Modified-Since,
 * replace the response with HttpNotModified. Add an ETag header if needed.
 */

#pragma once

// Module definitions
#include "./_def_.h"

// Framework libraries.
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: ConditionalGet
// TODO: docs for 'ConditionalGet'
class ConditionalGet : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::ConditionalGet";

protected:

	// Return true if an ETag header should be added to response.
	static bool needs_etag(http::abc::IHttpResponse* response);

public:
	inline explicit ConditionalGet(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	http::result_t process_response(http::Request* request, http::abc::IHttpResponse* response) override;
};

__MIDDLEWARE_END__
