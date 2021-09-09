/**
 * middleware/clickjacking.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Set the X-Frame-Options HTTP header in HTTP responses.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./types.h"
#include "./base.h"


__MIDDLEWARE_BEGIN__

// TESTME: XFrameOptions
/** Set the X-Frame-Options HTTP header in HTTP responses.
 *
 * Do not set the header if it's already set or if the response contains
 * a xframe_options_exempt value set to `true`.
 *
 * By default, set the X-Frame-Options header to 'SAMEORIGIN', meaning the
 * response can only be loaded on a frame within the same site. To prevent the
 * response from being loaded in a frame in any site, make `setting->get_x_frame_options()` in
 * your project's settings return `xw::conf::XFrameOptions::Deny`.
 */
class XFrameOptions : public MiddlewareWithConstantSettings
{
public:
	static inline constexpr const char* NAME = "xw::middleware::XFrameOptions";

	explicit inline XFrameOptions(const conf::Settings* settings) :
		MiddlewareWithConstantSettings(settings)
	{

	}

	virtual Function operator() (const Function& next);

	// Get the value to set for the X-Frame-Options header. Use the value from
	// the `setting->get_x_frame_options()` result.
	//
	// This method can be overridden if needed, allowing it to vary based on
	// the `request` or `response`.
	virtual inline std::string get_x_frame_options_value(http::Request* request, http::abc::IHttpResponse* response)
	{
		return this->settings->X_FRAME_OPTIONS.to_string();
	}
};

__MIDDLEWARE_END__
