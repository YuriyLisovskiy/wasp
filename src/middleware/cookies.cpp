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

/*
 * CookieMiddleware implementation.
 * TODO: write docs.
 */

#include "cookies.h"


__WASP_BEGIN__

void CookieMiddleware::processRequest(wasp::HttpRequest& request)
{
	if (request.headers.contains("Cookie"))
	{
		auto* cookies = internal::CookieParser::parseRequestCookies(request.headers.get("Cookie"));
		request.COOKIES = QueryDict(*cookies);
		delete cookies;
	}
}

void CookieMiddleware::processResponse(const wasp::HttpRequest& request, wasp::HttpResponse& response)
{
}

__WASP_END__