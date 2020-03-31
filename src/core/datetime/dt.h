/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * core/datetime/dt.h
 *
 * Purpose:
 *  C++ implementation of Python's datetime utilities.
 */

#pragma once

// C++ libraries.
#include <ctime>
#include <iostream>
#include <string>

// Module definitions.
#include "./_def_.h"

// Framework modules.
// TODO:


__DATETIME_BEGIN__

enum time_spec
{
	AUTO, HOURS, MINUTES, SECONDS, MILLISECONDS, MICROSECONDS
};

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

void __M_Assert(const char* expr_str, bool expr, const char* function, int line, const char* msg)
{
	if (!expr)
	{
		std::cerr << function << ": assertion failed: '"  << expr_str << "'\n\t" << msg << "\n";
		abort();
	}
}

#define m_assert(_expr, _msg) \
	__M_Assert(#_expr, _expr, __PRETTY_FUNCTION__, __LINE__, _msg)

template <typename _T>
signed char _cmp(_T left, _T right)
{
	if (left == right)
	{
		return 0;
	}

	return left > right ? 1 : -1;
}

const uint _MAX_ORDINAL = 3652059;

const signed char _MONTHS_COUNT = 12;

const signed char _DAYS_IN_MONTH[_MONTHS_COUNT + 1] = {
	-1, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

short int _DAYS_BEFORE_MONTH[_MONTHS_COUNT + 1] = {
	-1,             // a placeholder for indexing purposes
	0,              // Jan
	31,             // Feb
	31+28,          // Mar
	2*31+28,        // Apr
	2*31+28+30,     // May
	3*31+28+30,     // Jun
	3*31+28+2*30,   // Jul
	4*31+28+2*30,   // Aug
	5*31+28+2*30,   // Sep
	5*31+28+3*30,   // Oct
	6*31+28+3*30,   // Nov
	6*31+28+4*30    // Dec
};

// year -> true if leap year, else false.
extern bool _is_leap(ushort year);

// year -> number of days before January 1st of year.
extern size_t _days_before_year(ushort year);

// year, month -> number of days in that month in that year.
extern int _days_in_month(ushort year, ushort month);

// year, month -> number of days in year preceding first day of month.
extern size_t _days_before_month(ushort year, ushort month);

// year, month, day -> ordinal, considering 01-Jan-0001 as day 1.
extern size_t _ymd2ord(ushort year, ushort month, ushort day);

const size_t _DI400Y = _days_before_year(401);  // number of days in 400 years
const size_t _DI100Y = _days_before_year(101);  // ----------------- 100 -----
const size_t _DI4Y = _days_before_year(5);      // -----------------   4 -----

struct ymd
{
	ushort year;
	ushort month;
	ushort day;
};

struct hmsf
{
	ushort hh;
	ushort mm;
	ushort ss;
	uint ff;
};

// ordinal -> ymd{year, month, day}, considering 01-Jan-0001 as day 1.
extern ymd _ord2ymd(size_t n);

// Month and day names.  For localized versions, see the calendar module.
const std::string _MONTH_NAMES[_MONTHS_COUNT + 1] = {
	"", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
const std::string _DAY_NAMES[7 + 1] = {
	"", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

extern tm _build_struct_time(
	ushort y, ushort m, ushort d, ushort hh, ushort mm, ushort ss, int dst_flag
);

// Prepends 'c' 'w'-times and appends 'num' as std::string;
// if 'num' string is longer than 'w', returns 'num' as std::string.
extern std::string _lf(int num, int w = 2, char c = '0');

extern std::string _format_time(
	ushort hh, ushort mm, ushort ss, uint us, time_spec ts = time_spec::AUTO
);

// TODO:
//  std::string _format_offset();

// TODO:
//  _wrap_strftime(object, format, timetuple);

// Helpers for parsing the result of isoformat()
extern ymd _parse_isoformat_date(const std::string& dt_str);

extern hmsf _parse_hh_mm_ss_ff(const std::string& t_str);

// TODO:
//  _parse_isoformat_time(t_str);

// TODO:
//  _check_utc_offset(name, offset)

extern void _check_date_fields(ushort year, ushort month, ushort day);

extern void _check_time_fields(
	ushort hour, ushort minute, ushort second, uint microsecond, unsigned char fold
);

// Divide a by b and round result to the nearest integer.
//
// When the ratio is exactly half-way between two integers,
// the even integer is returned.
extern int _divide_and_round(double a, double b);

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

#ifndef ushort
typedef unsigned short int ushort;
#endif

#ifndef uint
typedef unsigned int uint;
#endif

#ifndef ullint
typedef unsigned long long int ullint;
#endif

const ushort MIN_YEAR = 1;
const ushort MAX_YEAR = 9999;

// Represent the difference between two datetime objects.
//
// Supported operators:
//
// - add, subtract timedelta
// - unary plus, minus, abs
// - compare to timedelta
// - multiply, divide by int
//
// In addition, datetime supports subtraction of two datetime objects
// returning a timedelta, and addition or subtraction of a datetime
// and a timedelta giving a datetime.
//
// Representation: (days, seconds, microseconds).
class Timedelta final
{
private:
	uint _days;
	uint _seconds;
	uint _microseconds;
	uint _hashcode;

private:
	[[nodiscard]] std::string _plural(uint n) const;

public:
	explicit Timedelta(
		uint days = 0, ullint seconds = 0, ullint microseconds = 0,
		ullint milliseconds = 0, uint minutes = 0, uint hours = 0, uint weeks = 0
	);
	[[nodiscard]] std::string str() const;

	// Total seconds in the duration.
	[[nodiscard]] ullint total_seconds() const;

	// Read-only field accessors.
	[[nodiscard]] uint days() const;
	[[nodiscard]] uint seconds() const;
	[[nodiscard]] uint microseconds() const;

	// TODO: impl arithmetic and conditional ops
};

__DATETIME_END__
