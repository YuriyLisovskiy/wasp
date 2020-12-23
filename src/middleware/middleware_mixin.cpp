/**
 * middleware/middleware_mixin.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./middleware_mixin.h"


__MIDDLEWARE_BEGIN__

MiddlewareMixin::MiddlewareMixin(conf::Settings* settings)
{
	this->settings = settings;
}

std::unique_ptr<http::IHttpResponse> MiddlewareMixin::process_request(http::HttpRequest* request)
{
	return nullptr;
}

std::unique_ptr<http::IHttpResponse> MiddlewareMixin::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	return nullptr;
}

__MIDDLEWARE_END__
