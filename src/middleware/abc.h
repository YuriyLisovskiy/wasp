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

// Base libraries.
#include <xalwart.base/result.h>

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
	virtual http::result_t process_request(http::HttpRequest* request) = 0;

	// An output http request and response after processing in controller.
	virtual http::result_t process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) = 0;
};

template <typename T>
concept middleware_type_c = std::is_base_of_v<IMiddleware, T>;

__MIDDLEWARE_END__
