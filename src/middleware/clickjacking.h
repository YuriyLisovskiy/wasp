/**
 * middleware/clickjacking.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Set the X-Frame-Options HTTP header in HTTP responses.
 *
 * Do not set the header if it's already set or if the response contains
 * a xframe_options_exempt value set to true.
 *
 * By default, set the X-Frame-Options header to 'SAMEORIGIN', meaning the
 * response can only be loaded on a frame within the same site. To prevent the
 * response from being loaded in a frame in any site, set X_FRAME_OPTIONS in
 * your project's settings to 'DENY'.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: XFrameOptions
// TODO: docs for 'XFrameOptions'
class XFrameOptions : public BaseMiddleware
{
public:
	inline static const std::string FULL_NAME = "xw::middleware::XFrameOptions";

	inline explicit XFrameOptions(conf::Settings* settings) : BaseMiddleware(settings)
	{
	}

	std::unique_ptr<http::abc::IHttpResponse> process_response(
		http::Request* request, http::abc::IHttpResponse* response
	) override;

	// Get the value to set for the X_FRAME_OPTIONS header. Use the value from
	// the X_FRAME_OPTIONS setting, or 'DENY' if not set.
	//
	// This method can be overridden if needed, allowing it to vary based on
	// the request or response.
	inline virtual std::string get_x_frame_options_value(http::Request* request, http::abc::IHttpResponse* response)
	{
		return this->settings->X_FRAME_OPTIONS.empty() ? "DENY" : this->settings->X_FRAME_OPTIONS;
	}
};

__MIDDLEWARE_END__
