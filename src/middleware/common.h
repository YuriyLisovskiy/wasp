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

// TESTME: CommonMiddleware
class CommonMiddleware : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::CommonMiddleware";

protected:
	virtual inline Result<std::shared_ptr<http::IHttpResponse>> get_response_redirect(
		const std::string& redirect_to
	)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<http::HttpResponsePermanentRedirect>(redirect_to)
		);
	}

	// Return true if settings.APPEND_SLASH is true and appending a slash to
	// the request path turns an invalid path into a valid one.
	bool should_redirect_with_slash(http::HttpRequest* request);

	// Return the full path of the request with a trailing slash appended.
	// Raise a RuntimeError if settings.DEBUG is true and request method is
	// POST, PUT, or PATCH.
	Result<std::string> get_full_path_with_slash(http::HttpRequest* request);

public:
	inline explicit CommonMiddleware(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	// Check for denied User-Agents and rewrite the URL based on
	// settings.APPEND_SLASH and settings.PREPEND_WWW
	Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override;

	// When the status code of the response is 404, it may redirect to a path
	// with an appended slash if should_redirect_with_slash() returns true.
	Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
