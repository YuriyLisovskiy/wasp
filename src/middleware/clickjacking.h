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
 * clickjacking.h
 *
 * Purpose:
 * 	Provides a middleware that implements protection against a
 * 	malicious site loading resources from your site in a hidden frame.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./middleware_mixin.h"
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

/**
 * Set the X-Frame-Options HTTP header in HTTP responses.
 *
 * Do not set the header if it's already set or if the response contains
 * a xframe_options_exempt value set to True.
 *
 * By default, set the X-Frame-Options header to 'SAMEORIGIN', meaning the
 * response can only be loaded on a frame within the same site. To prevent the
 * response from being loaded in a frame in any site, set X_FRAME_OPTIONS in
 * your project's Django settings to 'DENY'.
 */
class XFrameOptionsMiddleware : public MiddlewareMixin
{
public:
	explicit XFrameOptionsMiddleware(conf::Settings* settings);

	http::HttpResponseBase* process_response(http::HttpRequest* request, http::HttpResponseBase* response) override;

	/// Get the value to set for the X_FRAME_OPTIONS header. Use the value from
	/// the X_FRAME_OPTIONS setting, or 'DENY' if not set.
	///
	/// This method can be overridden if needed, allowing it to vary based on
	/// the request or response.
	virtual std::string get_x_frame_options_value(
		http::HttpRequest* request, http::HttpResponseBase* response
	);
};

__MIDDLEWARE_END__
