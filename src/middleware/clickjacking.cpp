/**
 * middleware/clickjacking.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./clickjacking.h"

// Framework libraries.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

const std::string XFrameOptionsMiddleware::FULL_NAME = "xw::middleware::XFrameOptionsMiddleware";

XFrameOptionsMiddleware::XFrameOptionsMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

Result<std::shared_ptr<http::IHttpResponse>> XFrameOptionsMiddleware::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	// Don't set it if it's already in the response.
	if (response->has_header(http::X_FRAME_OPTIONS))
	{
		return this->none();
	}

	response->set_header(http::X_FRAME_OPTIONS, this->get_x_frame_options_value(request, response));
	return this->none();
}

std::string XFrameOptionsMiddleware::get_x_frame_options_value(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	return this->settings->X_FRAME_OPTIONS.empty() ? "DENY" : this->settings->X_FRAME_OPTIONS;
}

__MIDDLEWARE_END__
