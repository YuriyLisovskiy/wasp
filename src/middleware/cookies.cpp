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
 * An implementation of cookies.h.
 */

#include "./cookies.h"


__MIDDLEWARE_BEGIN__

CookieMiddleware::CookieMiddleware(wasp::conf::Settings* settings)
	: MiddlewareMixin(settings)
{
}

void CookieMiddleware::process_request(http::HttpRequest* request)
{
	auto* cookies = core::internal::cookie_parser::parse(
		request->headers.get("Cookie", "")
	);
	request->COOKIES = collections::Dict(*cookies);
	delete cookies;
}

__MIDDLEWARE_END__
