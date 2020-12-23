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
#include "../conf/settings.h"


__MIDDLEWARE_BEGIN__

class MiddlewareMixin : public IMiddleware
{
protected:
	conf::Settings* settings;

public:
	explicit MiddlewareMixin(conf::Settings* settings);
	MiddlewareMixin(nullptr_t) = delete;
	~MiddlewareMixin() override = default;

	std::unique_ptr<http::IHttpResponse> process_request(http::HttpRequest* request) override;
	std::unique_ptr<http::IHttpResponse> process_response(
		http::HttpRequest* request, http::IHttpResponse* response
	) override;
};

__MIDDLEWARE_END__
