/**
 * utils/cache.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Utilities for developing middleware.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"


__UTIL_CACHE_INTERNAL_BEGIN__

// TESTME: if_match_passes
// Test the If-Match comparison as defined in section 3.1 of RFC 7232.
extern bool if_match_passes(const std::string& target_etag, std::vector<std::string>& etags);

// TESTME: if_none_match_passes
// Test the If-None-Match comparison as defined in section 3.2 of RFC 7232.
extern bool if_none_match_passes(const std::string& target_etag, std::vector<std::string>& etags);

// TESTME: if_modified_since_passes
// Test the If-Modified-Since comparison as defined in section 3.3 of RFC 7232.
inline bool if_modified_since_passes(long last_modified, long if_modified_since)
{
	return last_modified == -1 || last_modified > if_modified_since;
}

// TESTME: if_unmodified_since_passes
// Test the If-Unmodified-Since comparison as defined in section 3.4 of
// RFC 7232.
inline bool if_unmodified_since_passes(long last_modified, long if_unmodified_since)
{
	return last_modified != -1 && last_modified <= if_unmodified_since;
}

// TESTME: precondition_failed
// TODO: docs for 'precondition_failed'
inline std::shared_ptr<http::abc::IHttpResponse> precondition_failed(http::Request* request)
{
	return std::make_shared<http::Response>(412);
}

// TESTME: not_modified
// TODO: docs for 'not_modified'
extern std::shared_ptr<http::abc::IHttpResponse> not_modified(
	http::Request* request, http::abc::IHttpResponse* response
);

__UTIL_CACHE_INTERNAL_END__


__UTIL_CACHE_BEGIN__

// TESTME: set_response_etag
// TODO: docs for 'set_response_etag'
extern void set_response_etag(http::abc::IHttpResponse* response);

// TESTME: get_conditional_response
// TODO: docs for 'get_conditional_response'
extern std::shared_ptr<http::abc::IHttpResponse> get_conditional_response(
	http::Request* request,
	const std::string& etag,
	long last_modified,
	http::abc::IHttpResponse* response
);

__UTIL_CACHE_END__
