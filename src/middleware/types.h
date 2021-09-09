/**
 * middleware/types.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Middleware types.
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

using Function = std::function<std::unique_ptr<http::abc::IHttpResponse>(http::Request* request)>;
using Handler = std::function<Function(const Function&)>;

__MIDDLEWARE_END__
