/**
 * middleware/middleware_mixin.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: base middleware class.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class MiddlewareMixin : public IMiddleware
{
protected:
	conf::Settings* settings;

protected:
	Result<std::shared_ptr<http::IHttpResponse>> none();

	template<typename ResponseT, typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> result(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	Result<std::shared_ptr<http::IHttpResponse>> result(
		const std::shared_ptr<http::IHttpResponse>& response
	);

	template<error_type ErrorType, typename ...Args>
	Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return xw::raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	MiddlewareMixin(std::nullptr_t) = delete;
	~MiddlewareMixin() override = default;

	Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override;
	Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
