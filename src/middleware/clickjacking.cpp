/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * An implementation of middleware/clickjacking.h
 */

#include "./clickjacking.h"

// Framework modules.
#include "../http/headers.h"


__MIDDLEWARE_BEGIN__

const std::string XFrameOptionsMiddleware::FULL_NAME = "xw::middleware::XFrameOptionsMiddleware";

XFrameOptionsMiddleware::XFrameOptionsMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

std::unique_ptr<http::IHttpResponse> XFrameOptionsMiddleware::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	// Don't set it if it's already in the response.
	if (response->has_header(http::X_FRAME_OPTIONS))
	{
		return nullptr;
	}

	response->set_header(http::X_FRAME_OPTIONS, this->get_x_frame_options_value(request, response));
	return nullptr;
}

std::string XFrameOptionsMiddleware::get_x_frame_options_value(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	return this->settings->X_FRAME_OPTIONS.empty() ? "DENY" : this->settings->X_FRAME_OPTIONS;
}

__MIDDLEWARE_END__
