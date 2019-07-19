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
 * TODO: write docs.
 */

#include "cookie.h"


__WASP_BEGIN__

Cookie::Cookie() : Cookie("", "", "")
{
}

Cookie::Cookie(
	std::string name,
	std::string value,
	std::string expires,
	std::string domain,
	std::string path,
	bool isSecure,
	bool isHttpOnly
) : _name(std::move(name)), _value(std::move(value)), _expires(std::move(expires)),
	_domain(std::move(domain)), _path(std::move(path)),
	_isSecure(isSecure), _isHttpOnly(isHttpOnly)
{
	if (this->_name.empty())
	{
		throw CookieError("cookie's name can not be empty", _ERROR_DETAILS_);
	}
	if (this->_expires.empty())
	{
		throw CookieError("cookie's expiration time can not be empty", _ERROR_DETAILS_);
	}
}

std::string Cookie::toString()
{
	// Sets browser's directive 'Set-Cookie' and cookie's name and value.
	std::string result = "Set-Cookie: " + this->_name + "=" + this->_value;

	// Sets domain if it is not an empty string.
	result += !this->_domain.empty() ? "; Domain=" + this->_domain : "";

	// Sets path if it is not an empty string.
	result += !this->_path.empty() ? "; Path=" + this->_path : "";

	// Sets expiration time in '%a, %e %b %Y %T %Z' format,
	// for instance, 'Thu, 18 Jul 2019 16:25:19 GMT'.
	result += "; Expires=" + this->_expires;

	if (this->_isSecure)
	{
		result += "; Secure";
	}
	if (this->_isHttpOnly)
	{
		result += "; HttpOnly";
	}
	return result;
}

bool Cookie::operator==(const Cookie& right)
{
	return this->_name == right._name &&
			this->_value == right._value &&
			this->_domain == right._domain &&
			this->_path == right._path &&
			this->_expires == right._expires &&
			this->_isSecure == right._isSecure &&
			this->_isHttpOnly == right._isHttpOnly;
}

__WASP_END__
