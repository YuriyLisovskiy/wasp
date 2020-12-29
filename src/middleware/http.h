/**
 * middleware/http.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Handle conditional GET operations. If the response has an ETag or
 * 	Last-Modified header and the request has If-None-Match or If-Modified-Since,
 * 	replace the response with HttpNotModified. Add an ETag header if needed.
 */

#pragma once

// Module definitions
#include "./_def_.h"

// Framework libraries.
#include "./middleware_mixin.h"


__MIDDLEWARE_BEGIN__

class ConditionalGetMiddleware : public MiddlewareMixin
{
public:
	static const std::string FULL_NAME;

protected:

	/// Return true if an ETag header should be added to response.
	static bool needs_etag(http::IHttpResponse* response);

public:
	explicit ConditionalGetMiddleware(conf::Settings* settings);
	ConditionalGetMiddleware(nullptr_t) = delete;

	core::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
