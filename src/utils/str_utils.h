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

#ifndef LEAF_STR_UTILS_H
#define LEAF_STR_UTILS_H

#include <string>
#include <cstdarg>
#include <cctype>
#include <sstream>
#include <map>

#include "../globals.h"
#include "query_set.h"


__INTERNAL_BEGIN__

int normalize(double* val);

std::string _format(char const* fmt, va_list arg);

__INTERNAL_END__


__LEAF_BEGIN__

// ftoaFixed - carry out a fixed conversion of a double value to a string, with a precision of 5 decimal digits.
// Values with absolute values less than 0.000001 are rounded to 0.0
// Note: this blindly assumes that the buffer will be large enough to hold the largest possible result.
// The largest value we expect is an IEEE 754 double precision real, with maximum magnitude of approximately
// e+308. The C standard requires an implementation to allow a single conversion to produce up to 512
// characters, so that's what we really expect as the buffer size.
std::string ftoaFixed(double value);

// ftoaSci - converts double value to scientific number in string form.
std::string ftoaSci(double value);

// format - formats template with given arguments using internal::_format function.
std::string format(const char* fmt, ...);

// convert - converts string to type T.
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

__LEAF_END__


#endif // LEAF_STR_UTILS_H
