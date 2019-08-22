#include <utility>

#include <utility>

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
 * cookie definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_COOKIE_H
#define WASP_HTTP_COOKIE_H

#include <string>
#include <type_traits>

#include "../globals.h"
#include "../core/exceptions.h"


__WASP_BEGIN__

class Cookie
{
private:
	std::string _name;
	std::string _value;
	std::string _expires;
	std::string _domain;
	std::string _path;
	bool _isSecure;
	bool _isHttpOnly;

public:
	Cookie();
	Cookie(
		std::string name,
		std::string value,
		std::string expires,
		std::string domain = "",
		std::string path = "/",
		bool isSecure = true,
		bool isHttpOnly = false
	);

	std::string toString();

	bool operator==(const Cookie& right);
};

__WASP_END__


#endif // WASP_HTTP_COOKIE_H
