#include <utility>

#include <utility>

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
 * TODO: write docs.
 */

#include <iomanip>
#include "datetime.h"


__INTERNAL_BEGIN__

time_t nowAsTimeT()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

__INTERNAL_END__


__DATETIME_BEGIN__

DateTime now()
{
	auto timeNow = internal::nowAsTimeT();
	auto localTime = std::localtime(&timeNow);
	return DateTime(
		localTime->tm_year,
		localTime->tm_mon,
		localTime->tm_wday,
		localTime->tm_yday,
		localTime->tm_hour,
		localTime->tm_min,
		localTime->tm_sec,
		localTime->tm_sec * 1000000,
		TimeZone(timeNow)
	);
}

DateTime utcnow()
{
	auto timeNow = internal::nowAsTimeT();
	auto gmTime = std::gmtime(&timeNow);
	return DateTime(
		gmTime->tm_year,
		gmTime->tm_mon,
		gmTime->tm_wday,
		gmTime->tm_yday,
		gmTime->tm_hour,
		gmTime->tm_min,
		gmTime->tm_sec,
		gmTime->tm_sec * 1000000,
		TimeZone(timeNow)
	);
}


// DateTime class implementation.
DateTime::DateTime(int year, int month, int dayOfWeek, int dayOfYear, int hour, int minute, int second, int microsecond, TimeZone tz)
	: _date(Date(year, month, dayOfWeek, dayOfYear)), _time(Time(hour, minute, second, microsecond)), _tz(std::move(tz))
{
}

DateTime::DateTime(int year, int month, int dayOfWeek, int dayOfYear, int hour, int minute, int second, int microsecond, const std::string& tz)
	: DateTime(year, month, dayOfWeek, dayOfYear, hour, minute, second, microsecond, TimeZone(tz))
{
}

DateTime::DateTime(Date date, Time time, TimeZone tz) : _date(date), _time(time), _tz(std::move(tz))
{
}

DateTime::DateTime(Date date, Time time, const std::string& tz) : DateTime(date, time, TimeZone(tz))
{
}

int DateTime::year()
{
	return this->_date.year();
}

int DateTime::month()
{
	return this->_date.month();
}

int DateTime::dayOfWeek()
{
	return this->_date.dayOfWeek();
}

int DateTime::dayOfYear()
{
	return this->_date.dayOfYear();
}

int DateTime::hour()
{
	return this->_time.hour();
}

int DateTime::minute()
{
	return this->_time.minute();
}

int DateTime::second()
{
	return this->_time.second();
}

int DateTime::microsecond()
{
	return this->_time.microsecond();
}

TimeZone DateTime::tz()
{
	return this->_tz;
}

std::string DateTime::strftime(const char *_format)
{
	// TODO: implement method
	return "";
}

DateTime DateTime::strptime(const char *_datetime, const char *_format)
{
	// TODO: implement method
	return DateTime(0, 0, 0, 0, 0, 0, 0, 0, TimeZone());
}

__DATETIME_END__
