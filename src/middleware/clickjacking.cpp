/**
 * middleware/clickjacking.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./clickjacking.h"

// Framework libraries.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

http::result_t XFrameOptionsMiddleware::process_response(http::HttpRequest* request, http::IHttpResponse* response)
{
	// Set it if it's not already in the response.
	if (!response->has_header(http::X_FRAME_OPTIONS))
	{
		response->set_header(http::X_FRAME_OPTIONS, this->get_x_frame_options_value(request, response));
	}

	return {};
}

__MIDDLEWARE_END__
