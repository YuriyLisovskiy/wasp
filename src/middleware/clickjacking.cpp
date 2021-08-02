/**
 * middleware/clickjacking.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./clickjacking.h"

// Framework libraries.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

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

__MIDDLEWARE_END__
