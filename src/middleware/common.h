/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * common.h
 * Purpose:
 * "Common" middleware takes care of some basic operations:
 * 		- Forbid access to User-Agents in settings.DISALLOWED_USER_AGENTS
 *  	- URL rewriting: Based on the APPEND_SLASH and PREPEND_WWW settings,
 * 		append missing slashes and/or prepends missing "www."s.
 * 			- If APPEND_SLASH is set and the initial URL doesn't end with a
 * 		  	  slash, and it is not found in urlpatterns, form a new URL by
 * 		  	  appending a slash at the end. If this new URL is found in
 * 		  	  urlpatterns, return an HTTP redirect to this new URL; otherwise
 * 		  	  process the initial URL as usual.
 *
 * 		This behavior can be customized by subclassing CommonMiddleware and
 * 		overriding the get_response_redirect method.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./middleware_mixin.h"
#include "../core/string/str.h"
#include "../core/utility.h"
#include "../core/exceptions.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../http/headers.h"
#include "../http/utility.h"


__MIDDLEWARE_BEGIN__

class CommonMiddleware : public MiddlewareMixin
{
protected:
	virtual http::HttpResponseRedirectBase* get_response_redirect(
		const std::string& redirect_to
	);

	/// Return true if settings.APPEND_SLASH is true and appending a slash to
	/// the request path turns an invalid path into a valid one.
	bool should_redirect_with_slash(http::HttpRequest* request);

	/// Return the full path of the request with a trailing slash appended.
	/// Raise a RuntimeError if settings.DEBUG is true and request method is
	/// POST, PUT, or PATCH.
	std::string get_full_path_with_slash(http::HttpRequest* request);

public:
	explicit CommonMiddleware(conf::Settings* settings);

	/// Check for denied User-Agents and rewrite the URL based on
	/// settings.APPEND_SLASH and settings.PREPEND_WWW
	http::HttpResponseBase* process_request(http::HttpRequest* request) override;

	/// When the status code of the response is 404, it may redirect to a path
	/// with an appended slash if should_redirect_with_slash() returns true.
	http::HttpResponseBase* process_response(
		http::HttpRequest* request, http::HttpResponseBase* response
	) override;
};

__MIDDLEWARE_END__
