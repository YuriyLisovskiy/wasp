/**
 * middleware/middleware_mixin.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./middleware_mixin.h"


__MIDDLEWARE_BEGIN__

MiddlewareMixin::MiddlewareMixin(conf::Settings* settings)
{
	if (!settings)
	{
		throw core::NullPointerException(
			"middleware: settings is not instantiated", _ERROR_DETAILS_
		);
	}

	this->settings = settings;
}

core::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::none()
{
	return core::Result<std::shared_ptr<http::IHttpResponse>>();
}

core::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::result(
	const std::shared_ptr<http::IHttpResponse>& response
)
{
	return core::Result<std::shared_ptr<http::IHttpResponse>>(response);
}

core::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::process_request(http::HttpRequest* request)
{
	return this->none();
}

core::Result<std::shared_ptr<http::IHttpResponse>> MiddlewareMixin::process_response(
	http::HttpRequest* request, http::IHttpResponse* response
)
{
	return this->none();
}

__MIDDLEWARE_END__
