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
 * str.h
 * Purpose: contains helpful string's utilities.
 */

#ifndef WASP_UTILITY_STR_H
#define WASP_UTILITY_STR_H

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

/// Normalizes value to exponent.
///
/// @param val: pointer to value to normalize.
/// @return normalized value as int.
extern int normalize_exp(double* val);

/// Parses string and fill it with arguments list.
///
/// @param fmt: format template.
/// @param args: any values which can be put into string stream.
/// @return formatted string with given values.
extern std::string _format(char const* fmt, va_list args);

__UTILS_STR_INTERNAL_END__


__UTILS_STR_BEGIN__

/// Carries out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
/// Values with absolute values less than 0.000001 are rounded to 0.0
/// Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
/// The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
/// e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
/// characters, so that's what we really expect as the buffer size.
extern std::string ftoa_fixed(double value);

/// Converts double value to scientific number in string form.
///
/// @param value: value to convert
/// @return	std::string representation of converted value.
extern std::string ftoa_sci(double value);

/// Formats template with given arguments using internal::_format function.
///
/// @param fmt: format template.
/// @param ...: any values which can be put into string stream.
/// @return formatted string with given values.
extern std::string format(const char* fmt, ...);

/// Converts string to type _T.
///
/// @tparam _T: type to convert
/// @param str: value to convert which has overridden std::istringstream operator.
/// @return	converted std::string value to type _T
template<typename _T>
_T convert(const std::string& str)
{
	_T value;
	std::istringstream iss (str);
	iss >> value;
	if (iss.fail())
	{
		throw std::invalid_argument("can not convert \"" + str + "\"");
	}

	return value;
}

/// Joins std::vector of values separated by given delimiter.
///
/// @tparam _T: type of values of given std::vector.
/// @param delimiter: values' separator.
/// @param array: std::vector of values of type _T.
/// @return	joined string.
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

/// Joins wasp::Dict of values separated by given delimiter.
/// Each key-value pair can be formatted with some lambda expression
///		before beeing joined.
///
/// @tparam _Key: type of wasp::Dict keys.
/// @tparam _Val: type of wasp::Dict values.
/// @param delimiter: values' separator.
/// @param dict: wasp::Dict instance with key-value pairs.
/// @param expr: some expression which formats wasp::Dict item.
/// @return joined string.
template <typename _Key, typename _Val>
std::string join(
	const std::string& delimiter,
	Dict<_Key, _Val> dict,
	const std::function<std::string(const std::pair<_Key, _Val>&)>& expr = {}
)
{
	std::ostringstream oss;
	for (auto it = dict.cbegin(); it != dict.cend(); it++)
	{
		if (expr)
		{
			oss << expr(*it);
		}
		else
		{
			oss << (*it).first << delimiter << (*it).second;
		}

		if (std::next(it) != dict.cend())
		{
			oss << delimiter;
		}
	}

	return oss.str();
}

/// url_split_type("type:opaqueString", typeOut, opaqueStringOut) --> "type", "opaqueString".
///
/// @param url: url to analyze.
/// @param scheme: type output result.
/// @param data: opaque string output result.
extern void url_split_type(const std::string& url, std::string& scheme, std::string& data);

/// Checks if string contains char.
///
/// @param _str: string to analyze.
/// @param _char: symbol to search for in given string
/// @return true if symbol is found, otherwise returns false.
extern bool contains(const std::string& _str, char _char);

/// Converts all uppercase letters to lowercase.
///
/// @param _str: string with uppercase letters.
/// @return converted string.
extern std::string lower(const std::string& _str);

/// Converts all lowercase letters to uppercase.
///
/// @param _str: string with lowercase letters.
/// @return converted string.
extern std::string upper(const std::string& _str);

/// Splits the string vector of strings relatively for the character.
///
/// @param str: string to split.
/// @param delimiter: delimiter where to split string.
/// @return std::vector of strings.
extern std::vector<std::string> split(const std::string& str, char delimiter = ' ');

/// Checks if string starts with some string prefix.
///
/// @param src: string to check.
/// @param prefix: possible string beginning.
/// @return true if string starts with given prefix, otherwise returns false.
extern bool starts_with(const std::string& src, const std::string& prefix);

/// Trims left part of string in-place.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern void ltrim(std::string& s, char ch = ' ');

/// Trims right part of string in-place.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern void rtrim(std::string& s, char ch = ' ');

/// Trims both left and right parts of string in-place.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern void trim(std::string& s, char ch = ' ');

/// Trims left part of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern std::string ltrim(const std::string& s, char ch = ' ');

/// Trims right part of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern std::string rtrim(const std::string& s, char ch = ' ');

/// Trims both left and right parts of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param ch: char to be trimmed.
extern std::string trim(const std::string& s, char ch = ' ');

__UTILS_STR_END__


#endif // WASP_UTILITY_STR_H
