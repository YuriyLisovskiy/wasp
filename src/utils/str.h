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
 * str
 * TODO: write docs.
 */

#ifndef WASP_UTILS_STR_UTILS_H
#define WASP_UTILS_STR_UTILS_H

#include <string>
#include <cstdarg>
#include <cctype>
#include <sstream>
#include <map>
#include <vector>
#include <functional>
#include <regex>
#include <algorithm>

#include "../globals.h"
#include "../collections/dict.h"


__UTILS_STR_INTERNAL_BEGIN__

// Normalizes value to exponent.
extern int normalizeExp(double* val);

// Parses string and fill it with arguments list.
extern std::string _format(char const* fmt, va_list args);

__UTILS_STR_INTERNAL_END__


__UTILS_STR_BEGIN__

// Carries out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
// Values with absolute values less than 0.000001 are rounded to 0.0
// Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
// The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
// e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
// characters, so that's what we really expect as the buffer size.
extern std::string ftoaFixed(double value);

// Converts double value to scientific number in string form.
extern std::string ftoaSci(double value);

// Formats template with given arguments using internal::_format function.
extern std::string format(const char* fmt, ...);

// Converts string to type T.
template<typename T>
T convert(const std::string& str)
{
	T value;
	std::istringstream iss (str);
	iss >> value;
	if (iss.fail())
	{
		throw std::invalid_argument("can not convert \"" + str + "\"");
	}
	return value;
}

template <typename _T>
std::string join(const std::string& delimiter, std::vector<_T> array)
{
	std::string result;
	for (auto it = array.begin(); it != array.end(); it++)
	{
		result.append(std::to_string(*it));
		if (std::next(it) != array.end())
		{
			array.append(delimiter);
		}
	}
}

template <typename _Key, typename _Val>
std::string join(const std::string& delimiter, Dict<_Key, _Val> _qd, std::function<std::string(std::pair<_Key, _Val>)> expr)
{
	std::string result;
	for (auto it = _qd.cbegin(); it != _qd.cend(); it++)
	{
		result.append(expr(*it));
		if (std::next(it) != _qd.cend())
		{
			result.append(delimiter);
		}
	}
}

// urlSplitType("type:opaqueString", typeOut, opaqueStringOut) --> "type", "opaqueString".
extern void urlSplitType(const std::string& url, std::string& scheme, std::string& data);

// Checks if string contains char.
extern bool contains(const std::string& _str, char _char);

extern std::string lower(const std::string& _str);

__UTILS_STR_END__


#endif // WASP_UTILS_STR_UTILS_H
