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
 * CookieMiddleware definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_MIDDLEWARE_COOKIE_MIDDLEWARE_H
#define WASP_HTTP_MIDDLEWARE_COOKIE_MIDDLEWARE_H

#include <map>
#include <string>

#include "../../globals.h"
#include "BaseMiddleware.h"
#include "../../utils/query_dict.h"


__WASP_BEGIN__

class CookieMiddleware : protected BaseMiddleware
{
private:
	std::map<std::string, std::string> _parseCookies(const std::string& content);

	enum State
	{

	};

public:
	void modify(HttpRequest& request) final;
};

__WASP_END__


#endif // WASP_HTTP_MIDDLEWARE_COOKIE_MIDDLEWARE_H
