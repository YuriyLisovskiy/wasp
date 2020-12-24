/**
 * middleware/interface.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:	provides interfaces for middleware module.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"
#include "../http/result.h"


__MIDDLEWARE_BEGIN__

class IMiddleware
{
public:
	virtual ~IMiddleware() = default;

	/// An input http request before processing in views::View.
	virtual http::Result<std::shared_ptr<http::IHttpResponse>> process_request(
		http::HttpRequest* request
	) = 0;

	/// An output http request and response after processing in views::View.
	virtual http::Result<std::shared_ptr<http::IHttpResponse>> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) = 0;
};

__MIDDLEWARE_END__
