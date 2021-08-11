/**
 * middleware/abc.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Abstract base classes for middleware module.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../http/request.h"
#include "../http/response.h"


__MIDDLEWARE_BEGIN__

class IMiddleware
{
public:
	virtual ~IMiddleware() = default;

	// An input http request before processing in controller.
	virtual http::Response::Result process_request(http::Request* request) = 0;

	// An output http request and response after processing in controller.
	virtual http::Response::Result process_response(
		http::Request* request, http::abc::IHttpResponse* response
	) = 0;
};

template <typename T>
concept middleware_type = std::is_base_of_v<IMiddleware, T>;

__MIDDLEWARE_END__
