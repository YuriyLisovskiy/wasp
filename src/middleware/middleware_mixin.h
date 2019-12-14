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
 * cookie.h
 * Purpose: base middleware class.
 */

#ifndef WASP_MIDDLEWARE_MIDDLEWARE_MIXIN_H
#define WASP_MIDDLEWARE_MIDDLEWARE_MIXIN_H

#include "../globals.h"
#include "../http/request.h"
#include "../http/response.h"


__WASP_BEGIN__

class MiddlewareMixin
{
public:

	/// An input http request before processing in wasp::View.
	virtual void process_request(HttpRequest* request)
	{
	};

	/// An output http request and response after processing in wasp::View.
	virtual void process_response(const HttpRequest* request, HttpResponse* response)
	{
	};
};

__WASP_END__


#endif // WASP_MIDDLEWARE_MIDDLEWARE_MIXIN_H
