/**
 * middleware/common.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * "Common" middleware takes care of some basic operations:
 *  - Forbid access to User-Agents in settings.DISALLOWED_USER_AGENTS
 *  - URL rewriting: Based on the APPEND_SLASH and PREPEND_WWW settings,
 * 	  append missing slashes and/or prepends missing "www."s.
 * 	- If APPEND_SLASH is set and the initial URL doesn't end with a
 * 	  slash, and it is not found in urlpatterns, form a new URL by
 * 	  appending a slash at the end. If this new URL is found in
 * 	  urlpatterns, return an HTTP redirect to this new URL; otherwise
 * 	  process the initial URL as usual.
 *
 * This behavior can be customized by subclassing CommonMiddleware and
 * overriding the get_response_redirect method.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: Common
// TODO: docs for 'Common'
class Common : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::Common";

protected:
	virtual inline std::unique_ptr<http::abc::IHttpResponse> get_response_redirect(
		const std::string& redirect_to
	)
	{
		return std::make_unique<http::resp::PermanentRedirect>(redirect_to);
	}

	// Return true if settings.APPEND_SLASH is true and appending a slash to
	// the request path turns an invalid path into a valid one.
	bool should_redirect_with_slash(http::Request* request);

	// Return the full path of the request with a trailing slash appended.
	// Raise a RuntimeError if settings.DEBUG is true and request method is
	// POST, PUT, or PATCH.
	std::string get_full_path_with_slash(http::Request* request);

public:
	inline explicit Common(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	// Check for denied User-Agents and rewrite the URL based on
	// settings.APPEND_SLASH and settings.PREPEND_WWW
	std::unique_ptr<http::abc::IHttpResponse> process_request(http::Request* request) override;

	// When the status code of the response is 404, it may redirect to a path
	// with an appended slash if should_redirect_with_slash() returns true.
	std::unique_ptr<http::abc::IHttpResponse> process_response(
		http::Request* request, http::abc::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
