/**
 * utils/cache.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: provides utils for developing middleware.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"


__CACHE_INTERNAL_BEGIN__

/// Test the If-Match comparison as defined in section 3.1 of RFC 7232.
extern bool _if_match_passes(
	const std::string& target_etag, std::vector<std::string>& etags
);

/// Test the If-None-Match comparison as defined in section 3.2 of RFC 7232.
extern bool _if_none_match_passes(
	const std::string& target_etag, std::vector<std::string>& etags
);

/// Test the If-Modified-Since comparison as defined in section 3.3 of RFC 7232.
extern bool _if_modified_since_passes(
	long last_modified, long if_modified_since
);

/// Test the If-Unmodified-Since comparison as defined in section 3.4 of
/// RFC 7232.
extern bool _if_unmodified_since_passes(
	long last_modified, long if_unmodified_since
);

extern std::shared_ptr<http::IHttpResponse> _precondition_failed(
	http::HttpRequest* request
);

extern std::shared_ptr<http::IHttpResponse> _not_modified(
	http::HttpRequest* request, http::IHttpResponse* response
);

__CACHE_INTERNAL_END__


__CACHE_BEGIN__

extern void set_response_etag(http::IHttpResponse* response);

extern std::shared_ptr<http::IHttpResponse> get_conditional_response(
	http::HttpRequest* request,
	const std::string& etag,
	long last_modified,
	http::IHttpResponse* response
);

__CACHE_END__
