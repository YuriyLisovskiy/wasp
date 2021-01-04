/**
 * core/server/func.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <functional>

// Core libraries.
#include <xalwart.core/result.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../http/request.h"
#include "../../http/interfaces.h"


__SERVER_BEGIN__

typedef std::function<void(
	const http::HttpRequest*, const core::Result<std::shared_ptr<http::IHttpResponse>>&
)> StartResponseFunc;

typedef std::function<
	void(http::HttpRequest*, const StartResponseFunc&)
> HttpHandlerFunc;

__SERVER_END__
