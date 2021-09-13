/**
 * middleware/http.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Handle conditional GET operations.
 */

#pragma once

// Module definitions
#include "./_def_.h"

// Framework libraries.
#include "./types.h"


__MIDDLEWARE_BEGIN__

// TESTME: ConditionalGet
/** Handle conditional GET operations. If the response has an ETag or
 * Last-Modified header and the request has If-None-Match or If-Modified-Since,
 * replace the response with HttpNotModified. Add an ETag header if needed.
 */
class ConditionalGet
{
public:
	static inline constexpr const char* NAME = "xw::middleware::ConditionalGet";

	ConditionalGet() = default;

	virtual Function operator() (const Function& next) const;

protected:
	// Return true if an ETag header should be added to response.
	[[nodiscard]]
	virtual bool needs_etag(const std::unique_ptr<http::abc::IHttpResponse>& response) const;
};

__MIDDLEWARE_END__
