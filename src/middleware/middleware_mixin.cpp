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

http::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::none()
{
	return http::Result<std::shared_ptr<http::IHttpResponse>>();
}

http::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::result(
	const std::shared_ptr<http::IHttpResponse>& response
)
{
	return http::Result<std::shared_ptr<http::IHttpResponse>>(response);
}

http::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::process_request(http::HttpRequest* request)
{
	return this->none();
}

http::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	return this->none();
}

__MIDDLEWARE_END__
