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

	inline http::Response::Result process_request(http::Request* request) override
	{
		return {};
	}

	inline http::Response::Result process_response(http::Request* request, http::abc::IHttpResponse* response) override
	{
		return {};
	}
};

__MIDDLEWARE_END__
