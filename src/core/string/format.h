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
 * format.h
 * Purpose: utility for string's formatting.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <cstdarg>
#include <sstream>

// Module definitions.
#include "./_def_.h"


__STR_BEGIN__

/// Formats template with given arguments using internal::_format function.
///
/// @param fmt: format template.
/// @param ...: any values which can be put into string stream.
/// @return formatted string with given values.
extern std::string format(const char* fmt, ...);

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

__STR_END__


__STR_INTERNAL_BEGIN__

/// Parses string and fill it with arguments list.
///
/// @param fmt: format template.
/// @param args: any values which can be put into string stream.
/// @return formatted string with given values.
extern std::string _format(char const* fmt, va_list args);

/// Normalizes value to exponent.
///
/// @param val: pointer to value to normalize.
/// @return normalized value as int.
extern int normalize_exp(double* val);

__STR_INTERNAL_END__
