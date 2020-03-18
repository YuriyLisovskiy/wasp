/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * middleware/cookies.h
 *
 * Purpose:
 * 	Enables cookies in http request object.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./middleware_mixin.h"
#include "../collections/dict.h"
#include "../core/parsers/cookie_parser.h"


__MIDDLEWARE_BEGIN__

class CookieMiddleware final: public MiddlewareMixin
{
public:
	explicit CookieMiddleware(conf::Settings* settings);
	std::unique_ptr<http::IHttpResponse> process_request(
		http::HttpRequest* request
	) final;
};

__MIDDLEWARE_END__
