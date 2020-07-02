/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <xalwart/middleware/middleware_mixin.h>

class UsefulMiddleware final : public xw::middleware::MiddlewareMixin
{
public:
	UsefulMiddleware(xw::conf::Settings* settings);
	UsefulMiddleware(nullptr_t) = delete;

	std::unique_ptr<xw::http::IHttpResponse> process_request(
		xw::http::HttpRequest* request
	) override;
};
