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
	core::Result<std::shared_ptr<http::IHttpResponse>> none();

	template<typename ResponseT, typename ...Args>
	core::Result<std::shared_ptr<http::IHttpResponse>> result(Args&& ...args)
	{
		return core::Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	core::Result<std::shared_ptr<http::IHttpResponse>> result(
		const std::shared_ptr<http::IHttpResponse>& response
	);

	template<core::error_type ErrorType, typename ...Args>
	core::Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return core::raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	MiddlewareMixin(std::nullptr_t) = delete;
	~MiddlewareMixin() override = default;

	core::Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override;
	core::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
