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


__INTERNAL_BEGIN__

template <class _Val>
class Cookie
{
	static_assert(std::is_fundamental<_Val>::value, "cookie value must be of built-in type");
private:
	std::string _name;
	_Val _value;
	std::string _expires;
	std::string _domain;
	std::string _path;
	bool _isSecure;
	bool _isHttpOnly;

public:
	Cookie(
		std::string name,
		_Val value,
		std::string expires,
		std::string domain = "",
		std::string path = "/",
		bool isSecure = true,
		bool isHttpOnly = false
	) : _name(std::move(name)), _value(value), _expires(std::move(expires)),
	    _domain(std::move(domain)), _path(std::move(path)),
	    _isSecure(isSecure), _isHttpOnly(isHttpOnly)
	{
	}

	std::string toString()
	{
		// Sets browser's directive 'Set-Cookie' and cookie's name and value.
		std::string result = "Set-Cookie: " + this->_name + "=" + std::to_string(this->_value) + ";";

		// Sets domain if it is not an empty string.
		result += this->_domain.size() > 0 ? " Domain=" + this->_domain + ";" : "";

		// Sets path if it is not an empty string.
		result += this->_path.size() > 0 ? " Path=" + this->_path + ";" : "";

		// Sets expiration time in '%a, %e %b %Y %T %Z' format,
		// for instance, 'Thu, 18 Jul 2019 16:25:19 GMT'.
		result += " Expires=" + this->_expires + ";";

		if (this->_isSecure)
		{
			result += " Secure;";
		}
		if (this->_isHttpOnly)
		{
			result += " HttpOnly";
		}
		return result;
	}
};

__INTERNAL_END__


#endif // WASP_HTTP_COOKIE_H
