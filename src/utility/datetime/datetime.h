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
 * datetime
 * TODO: write docs.
 */

#ifndef WASP_UTILS_DATETIME_DATETIME_H
#define WASP_UTILS_DATETIME_DATETIME_H

#include <chrono>
#include <ctime>
#include <string>
#include <map>

#include "../../globals.h"
#include "date.h"
#include "time.h"
#include "timezone.h"


__DATETIME_BEGIN__


// A combination of a Date, Time and a TimeZone.
//
// Properties: _date, _time and _tz.
class DateTime
{
private:
	Date _date;
	Time _time;
	TimeZone _tz;

public:
	DateTime();
	DateTime(int year, int month, int dayOfWeek, int dayOfMonth, int dayOfYear, int hour, int minute, int second, int microsecond, TimeZone tz);
	DateTime(int year, int month, int dayOfWeek, int dayOfMonth, int dayOfYear, int hour, int minute, int second, int microsecond, const std::string& tz);
	DateTime(Date date, Time time, TimeZone tz);
	DateTime(Date date, Time time, const std::string& tz);
	explicit DateTime(time_t timestamp);

	int year();
	int month();
	int dayOfWeek();
	int dayOfYear();
	int hour();
	int minute();
	int second();
	int microsecond();
	Date date();
	Time time();
	TimeZone tz();

	std::string strftime(const char* _format);
	static DateTime strptime(const char* _datetime, const char* _format);
};


// Returns current date and time with local time zone.
DateTime now();

// Returns current date and time as utc time.
DateTime gmtnow();

__DATETIME_END__


__INTERNAL_BEGIN__

// Returns current date and time as time_t
time_t now();

__INTERNAL_END__


#endif // WASP_UTILS_DATETIME_DATETIME_H
