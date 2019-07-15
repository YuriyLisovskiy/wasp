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

#ifndef WASP_UTILS_DATETIME_H
#define WASP_UTILS_DATETIME_H

#include <chrono>
#include <ctime>
#include <string>
#include <map>

#include "../globals.h"

#define __DATETIME_BEGIN__ __WASP_BEGIN__ namespace dt {
#define __DATETIME_END__ } __WASP_END__


__DATETIME_BEGIN__

template <typename _TimeT = std::chrono::milliseconds>
class Measure
{
private:
	std::chrono::high_resolution_clock::time_point _begin;
	std::chrono::high_resolution_clock::time_point _end;

public:
	void start();
	void end();
	double elapsed(bool reset = true);
	void reset();
};

// The smallest year number allowed in a date or datetime object.
const int MIN_YEAR = 1;

// The largest year number allowed in a date or datetime object.
const int MAX_YEAR = 9999;


// An idealized naive date, assuming the current Gregorian calendar
// always was, and always will be, in effect.
//
// Getters: year(), month(), dayOfYear() and dayOfWeek().
class Date
{
private:
	int _year;
	int _month;
	int _dayOfWeek;
	int _dayOfYear;

public:
	Date() = default;
	Date(int year, int month, int dayOfWeek, int dayOfYear);

	int year();
	int month();
	int dayOfWeek();
	int dayOfYear();
};


// An idealized time, independent of any particular day, assuming that
// every day has exactly 24*60*60 seconds (there is no notion of “leap seconds” here).
//
// Getters: hour(), minute(), second() and microsecond().
class Time
{
private:
	int _hour;
	int _minute;
	int _second;
	int _microsecond;

public:
	Time() = default;
	Time(int hour, int minute, int second, int microsecond);

	int hour();
	int minute();
	int second();
	int microsecond();
};


// A class that represents a fixed offset from the UTC.
//
// Getters: hours(), minutes(), seconds() and microseconds()
class TimeZone
{
private:
	int _seconds;
public:
	enum OffsetAs
	{
		HOURS,
		MINUTES,
		SECONDS,
		MICROSECONDS
	};

	TimeZone() = default;
	explicit TimeZone(time_t when);

	// offset parameters is date and time offset in seconds.
	explicit TimeZone(int offset);

	int getOffset(OffsetAs offsetAs);
};


// A combination of a Date and a Time.
//
// Getters: year(), month(), day(), hour(), minute(), second(), microsecond(), and tz().
class DateTime
{
private:
	Date _date;
	Time _time;
	TimeZone _tz;

public:
	DateTime(int year, int month, int dayOfWeek, int dayOfYear, int hour, int minute, int second, int microsecond, TimeZone tz);
	DateTime(Date date, Time time);

	int year();
	int month();
	int day();
	int hour();
	int minute();
	int second();
	int microsecond();
	TimeZone tz();

	std::string strftime(const char* _format);
	static DateTime strptime(const char* _datetime, const char* _format);
};

// Returns current date and time as time_t
time_t nowAsTimeT();

// Returns current date and time with local time zone.
DateTime now();

// Returns current date and time as utc time.
DateTime utcnow();

__DATETIME_END__


#endif // WASP_UTILS_DATETIME_H
