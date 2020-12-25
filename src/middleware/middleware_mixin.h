/**
 * middleware/middleware_mixin.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: base middleware class.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./interfaces.h"
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class MiddlewareMixin : public IMiddleware
{
protected:
	conf::Settings* settings;

protected:
	http::Result<std::shared_ptr<http::IHttpResponse>> none();

	template<typename ResponseT, typename ...Args>
	http::Result<std::shared_ptr<http::IHttpResponse>> result(Args&& ...args)
	{
		return http::Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	http::Result<std::shared_ptr<http::IHttpResponse>> result(
		const std::shared_ptr<http::IHttpResponse>& response
	);

	template<http::error_type ErrorType, typename ...Args>
	http::Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return http::raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	MiddlewareMixin(nullptr_t) = delete;
	~MiddlewareMixin() override = default;

	http::Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override;
	http::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
