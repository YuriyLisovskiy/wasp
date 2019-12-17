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
 * cookies.h
 * Purpose: enables cookies in http request object.
 */

#ifndef WASP_MIDDLEWARE_COOKIE_MIDDLEWARE_H
#define WASP_MIDDLEWARE_COOKIE_MIDDLEWARE_H

#include "../globals.h"
#include "middleware_mixin.h"
#include "../http/request.h"
#include "../http/response.h"
#include "../collections/dict.h"
#include "../core/parsers/cookie_parser.h"


__WASP_BEGIN__

class CookieMiddleware final: public MiddlewareMixin
{
public:
	void process_request(wasp::http::HttpRequest* request) final;
};

__WASP_END__


#endif // WASP_MIDDLEWARE_COOKIE_MIDDLEWARE_H
