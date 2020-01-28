/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * interface.h
 * Purpose:
 * 	Provides interfaces for middleware module.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../http/request.h"
#include "../http/response.h"


__MIDDLEWARE_BEGIN__

class IMiddleware
{
public:
	virtual ~IMiddleware() = default;

	/// An input http request before processing in views::View.
	virtual http::HttpResponseBase* process_request(http::HttpRequest* request) = 0;

	/// An output http request and response after processing in views::View.
	virtual http::HttpResponseBase* process_response(
		http::HttpRequest* request, http::HttpResponseBase* response
	) = 0;
};

__MIDDLEWARE_END__
