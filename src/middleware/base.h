/**
 * middleware/middleware_mixin.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Base middleware class.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

// TESTME: BaseMiddleware
// TODO: docs for 'BaseMiddleware'
class BaseMiddleware : public IMiddleware
{
protected:
	conf::Settings* settings;

protected:
	inline Result<std::shared_ptr<http::IHttpResponse>> none()
	{
		return {};
	}

	template<typename ResponseT, typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> result(Args&& ...args)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(
			std::make_shared<ResponseT>(std::forward<Args>(args)...)
		);
	}

	inline Result<std::shared_ptr<http::IHttpResponse>> result(
		const std::shared_ptr<http::IHttpResponse>& response
	)
	{
		return Result<std::shared_ptr<http::IHttpResponse>>(response);
	}

	template<error_type ErrorType, typename ...Args>
	inline Result<std::shared_ptr<http::IHttpResponse>> raise(Args&& ...args)
	{
		return xw::raise<ErrorType, std::shared_ptr<http::IHttpResponse>>(
			std::forward<Args>(args)...
		);
	}

public:
	inline explicit BaseMiddleware(conf::Settings* settings)
	{
		if (!settings)
		{
			throw NullPointerException("middleware: settings is not instantiated", _ERROR_DETAILS_);
		}

		this->settings = settings;
	}

	~BaseMiddleware() override = default;

	inline Result<std::shared_ptr<http::IHttpResponse>> process_request(http::HttpRequest* request) override
	{
		return this->none();
	}

	inline Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override
	{
		return this->none();
	}
};

__MIDDLEWARE_END__
