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
 * datetime.h
 * Purpose: combines date, time and timezone in one class.
 */

#pragma once

// C++ libraries.
#include <map>
#include <ctime>
#include <string>
#include <sstream>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./date.h"
#include "./time.h"
#include "./timezone.h"
#include "./predefined/tz_abbr_to_offset.h"
#include "../regex.h"


__DATETIME_BEGIN__

/// A combination of a Date, Time and a TimeZone.
///
/// Properties: _date, _time and _tz.
class DateTime
{
private:
	Date _date;
	Time _time;
	TimeZone _tz;

public:
	DateTime();
	DateTime(int year, int month, int day, int hour, int minute, int second);
	DateTime(int year, int month, int day_of_week, int day_of_month, int day_of_year, int hour, int minute, int second, int microsecond, TimeZone tz);
	DateTime(int year, int month, int day_of_week, int day_of_month, int day_of_year, int hour, int minute, int second, int microsecond, const std::string& tz);
	DateTime(Date date, Time time, TimeZone tz);
	DateTime(Date date, Time time, const std::string& tz);
	explicit DateTime(time_t timestamp);

	Date date();
	Time time();
	TimeZone tz();
	size_t timestamp();

	std::string strftime(const char* _format);
	static DateTime strptime(const char* _datetime, const char* _format);

private:
	std::tm* _to_std_tm();
	std::string strf_tz(const char* _format);
};


/// Returns current date and time with local time zone.
DateTime now();

/// Returns current date and time as utc time.
DateTime gmtnow();

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

/// Returns current date and time as time_t
time_t now();

extern rgx::Regex RE_HOURS;

extern rgx::Regex RE_ZONE_ABBR;

__DATETIME_INTERNAL_END__
