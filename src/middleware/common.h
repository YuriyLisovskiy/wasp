/**
 * middleware/common.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Perform common operations with request and response.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./types.h"
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: Common
/** "Common" middleware takes care of some basic operations:
 *  - Forbid access to User-Agents in `settings->get_disallowed_user_agents()`
 *  - URL rewriting: Based on the `settings->APPEND_SLASH` and
 *    `settings->PREPEND_WWW` settings, append missing slashes and/or prepends
 *    missing "www."s.
 * 	- If `settings->APPEND_SLASH` is `true` and the initial URL doesn't end with a
 * 	  slash, and it is not found in urlpatterns, form a new URL by
 * 	  appending a slash at the end. If this new URL is found in
 * 	  urlpatterns, return an HTTP redirect to this new URL; otherwise
 * 	  process the initial URL as usual.
 *
 * This behavior can be customized by subclassing `Common` and
 * overriding the `get_response_redirect` method.
 */
class Common : public MiddlewareWithConstantSettings
{
public:
	static inline constexpr const char* NAME = "xw::middleware::Common";

	explicit inline Common(const conf::Settings* settings) : MiddlewareWithConstantSettings(settings)
	{
	}

	virtual Function operator() (const Function& next) const;

protected:
	[[nodiscard]]
	virtual inline std::unique_ptr<http::abc::HttpResponse> get_response_redirect(
		const std::string& redirect_to
	) const
	{
		return std::make_unique<http::PermanentRedirect>(redirect_to);
	}

	// Return `true` if `settings->APPEND_SLASH` is `true` and appending a slash to
	// the request path turns an invalid path into a valid one.
	virtual bool should_redirect_with_slash(http::Request* request) const;

	// Return the full path of the `request` with a trailing slash appended.
	// Throws a `RuntimeError` if `settings->DEBUG` is `true` and request method is
	// POST, PUT, or PATCH.
	virtual std::string get_full_path_with_slash(http::Request* request) const;

	// Check for denied User-Agents and rewrite the URL based on
	// `settings->APPEND_SLASH` and `settings->PREPEND_WWW`.
	virtual std::unique_ptr<http::abc::HttpResponse> preprocess(http::Request* request) const;

	// When the status code of the response is 404, it may redirect to a path
	// with an appended slash if `should_redirect_with_slash` returns `true`.
	virtual std::unique_ptr<http::abc::HttpResponse> postprocess(
		http::Request* request, http::abc::HttpResponse* response
	) const;
};

__MIDDLEWARE_END__
