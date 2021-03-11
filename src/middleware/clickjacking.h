/**
 * middleware/clickjacking.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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

// Framework libraries.
#include "./middleware_mixin.h"


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
 * your project's settings to 'DENY'.
 */
class XFrameOptionsMiddleware : public MiddlewareMixin
{
public:
	static const std::string FULL_NAME;

	explicit XFrameOptionsMiddleware(conf::Settings* settings);

	core::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;

	/// Get the value to set for the X_FRAME_OPTIONS header. Use the value from
	/// the X_FRAME_OPTIONS setting, or 'DENY' if not set.
	///
	/// This method can be overridden if needed, allowing it to vary based on
	/// the request or response.
	virtual std::string get_x_frame_options_value(
		http::HttpRequest* request, http::IHttpResponse* response
	);
};

__MIDDLEWARE_END__
