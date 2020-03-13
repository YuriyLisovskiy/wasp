/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/string/str.h
 *
 * Purpose: utility functions to simplify work with string.
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <functional>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../../collections/dict.h"


__STR_BEGIN__

/// Joins std::vector of values separated by given delimiter.
///
/// @tparam _Container: type of given container.
/// @param begin: constant iterator's pointer to the beginning of container.
/// @param end: constant iterator's pointer to the end of container.
/// @param delimiter: values' separator.
/// @return	joined string.
template <typename _Iterator>
std::string join(
	_Iterator begin,
	_Iterator end,
	const std::string& delimiter
)
{
	std::ostringstream oss;
	for (auto it = begin; it != end; it++)
	{
		oss << *it;
		if (std::next(it) != end)
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

/// Splits the string to a vector of strings relatively for the character.
///
/// @param str: string to split.
/// @param delimiter: delimiter where to split string.
/// @return std::vector of strings.
extern std::vector<std::string> split(const std::string& str, char delimiter = ' ');

/// Splits the string to a vector of strings with n length starting from right.
///
/// @param str: string to split.
/// @param delimiter: delimiter where to split string.
/// @return std::vector of strings.
extern std::vector<std::string> rsplit(const std::string& str, char delimiter = ' ', size_t n = -1);

/// Checks if string starts with some string prefix.
///
/// @param src: string to check.
/// @param prefix: possible string beginning.
/// @return true if string starts with given prefix, otherwise returns false.
extern bool starts_with(const std::string& src, const std::string& prefix);

/// Checks if string ends with some string suffix.
///
/// @param src: string to check.
/// @param suffix: possible string ending.
/// @return true if string ends with given suffix, otherwise returns false.
extern bool ends_with(const std::string& src, const std::string& suffix);

/// Trims left part of string in-place.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern void ltrim(std::string& s, const std::string& to_trim = " ");

/// Trims left part of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern std::string ltrim(const std::string& s, const std::string& to_trim = " ");

/// Trims right part of string in-place.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern void rtrim(std::string& s, const std::string& to_trim = " ");

/// Trims right part of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern std::string rtrim(const std::string& s, const std::string& to_trim = " ");

/// Trims both left and right parts of string in-place.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern void trim(std::string& s, const std::string& to_trim = " ");

/// Trims both left and right parts of string and returns a copy of trimmed string.
///
/// @param s: string to trim.
/// @param to_trim: string to be trimmed.
extern std::string trim(const std::string& s, const std::string& to_trim = " ");

extern size_t count(const std::string& src, char ch);

extern std::string cut_edges(
	const std::string& s, size_t left, size_t right, bool trim_whitespace = true
);

extern std::string replace(
	const std::string& src,
	const std::string& old_sub,
	const std::string& new_sub
);

extern std::string make_text_list(
	const std::vector<std::string>& list, const std::string& last
);

__STR_END__
