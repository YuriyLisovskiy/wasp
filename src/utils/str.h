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
extern int normalize_exp(double* val);

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
extern std::string ftoa_fixed(double value);

// Converts double value to scientific number in string form.
extern std::string ftoa_sci(double value);

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
	std::ostringstream oss;
	for (auto it = array.begin(); it != array.end(); it++)
	{
		oss << *it;
		if (std::next(it) != array.end())
		{
			oss << delimiter;
		}
	}
	return oss.str();
}

template <typename _Key, typename _Val>
std::string join(
	const std::string& delimiter,
	Dict<_Key, _Val> _qd,
	const std::function<std::string(const std::pair<_Key, _Val>&)>& expr = {}
)
{
	std::ostringstream oss;
	for (auto it = _qd.cbegin(); it != _qd.cend(); it++)
	{
		if (expr)
		{
			oss << expr(*it);
		}
		else
		{
			oss << (*it).first << delimiter << (*it).second;
		}
		if (std::next(it) != _qd.cend())
		{
			oss << delimiter;
		}
	}
	return oss.str();
}

// url_split_type("type:opaqueString", typeOut, opaqueStringOut) --> "type", "opaqueString".
extern void url_split_type(const std::string& url, std::string& scheme, std::string& data);

// Checks if string contains char.
extern bool contains(const std::string& _str, char _char);

extern std::string lower(const std::string& _str);

extern std::string upper(const std::string& _str);

// Splits the string vector of strings relatively for the character.
extern std::vector<std::string> split(const std::string& str, char delimiter = ' ');

extern bool starts_with(const std::string& src, const std::string& prefix);

// Trims left part of string in-place
extern void ltrim(std::string& s, char ch = ' ');

// Trims right part of string in-place
extern void rtrim(std::string& s, char ch = ' ');

// Trims both left and right parts of string in-place
extern void trim(std::string& s, char ch = ' ');

// Trims left part of string and returns a copy of trimmed string
extern std::string ltrim(const std::string& s, char ch = ' ');

// Trims right part of string and returns a copy of trimmed string
extern std::string rtrim(const std::string& s, char ch = ' ');

// Trims both left and right parts of string and returns a copy of trimmed string
extern std::string trim(const std::string& s, char ch = ' ');

__UTILS_STR_END__


#endif // WASP_UTILS_STR_UTILS_H
