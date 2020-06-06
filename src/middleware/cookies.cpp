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
 * An implementation of middleware/cookies.h
 */

#include "./cookies.h"

// Framework modules.
#include "../core/parsers/cookie_parser.h"


__MIDDLEWARE_BEGIN__

const std::string CookieMiddleware::FULL_NAME = "xw::middleware::CookieMiddleware";

CookieMiddleware::CookieMiddleware(conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

std::unique_ptr<http::IHttpResponse> CookieMiddleware::process_request(http::HttpRequest* request)
{
	core::internal::cookie_parser cp;
	cp.parse(request->headers.get("Cookie", ""));
	request->COOKIES = collections::Dict(cp.result);
	return nullptr;
}

__MIDDLEWARE_END__
