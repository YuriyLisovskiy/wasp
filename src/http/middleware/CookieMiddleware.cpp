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

#include "CookieMiddleware.h"


__WASP_BEGIN__

void CookieMiddleware::modify(wasp::HttpRequest& request)
{
	if (request.headers.contains("Cookie"))
	{
		request.COOKIES = QueryDict(this->_parseCookies(request.headers.get("Cookie")));
	}
}

std::map<std::string, std::string> CookieMiddleware::_parseCookies(const std::string& content)
{
	std::map<std::string, std::string> result;

	// TODO

	return result;
}

__WASP_END__
