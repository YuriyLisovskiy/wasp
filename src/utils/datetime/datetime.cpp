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

time_t now()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

__INTERNAL_END__


__DATETIME_BEGIN__

DateTime now()
{
	auto timeNow = internal::now();
	auto localTime = std::localtime(&timeNow);
	auto result = DateTime(
		localTime->tm_year,
		localTime->tm_mon,
		localTime->tm_wday,
		localTime->tm_mday,
		localTime->tm_yday,
		localTime->tm_hour,
		localTime->tm_min,
		localTime->tm_sec,
		localTime->tm_sec * 1000000,
		TimeZone(timeNow)
	);
	delete localTime;
	return result;
}

DateTime utcnow()
{
	auto timeNow = internal::now();
	auto gmTime = std::gmtime(&timeNow);
	auto result = DateTime(
		gmTime->tm_year,
		gmTime->tm_mon,
		gmTime->tm_wday,
		gmTime->tm_mday,
		gmTime->tm_yday,
		gmTime->tm_hour,
		gmTime->tm_min,
		gmTime->tm_sec,
		gmTime->tm_sec * 1000000,
		TimeZone(timeNow)
	);
	delete gmTime;
	return result;
}


// DateTime class implementation.
DateTime::DateTime() : _date(), _time(), _tz()
{
}

DateTime::DateTime(int year, int month, int dayOfWeek, int dayOfMonth, int dayOfYear, int hour, int minute, int second, int microsecond, TimeZone tz)
	: _date(Date(year, month, dayOfWeek, dayOfMonth, dayOfYear)), _time(Time(hour, minute, second, microsecond)), _tz(std::move(tz))
{
}

DateTime::DateTime(int year, int month, int dayOfWeek, int dayOfMonth, int dayOfYear, int hour, int minute, int second, int microsecond, const std::string& tz)
	: DateTime(year, month, dayOfWeek, dayOfMonth, dayOfYear, hour, minute, second, microsecond, TimeZone(tz))
{
}

DateTime::DateTime(Date date, Time time, TimeZone tz) : _date(date), _time(time), _tz(std::move(tz))
{
}

DateTime::DateTime(Date date, Time time, const std::string& tz) : DateTime(date, time, TimeZone(tz))
{
}

DateTime::DateTime(time_t timestamp) : _date(), _time(), _tz()
{
	auto localTime = std::localtime(&timestamp);
	this->_date = Date(
		localTime->tm_year,
		localTime->tm_mon,
		localTime->tm_wday,
		localTime->tm_mday,
		localTime->tm_yday
	);
	this->_time = Time(
		localTime->tm_hour,
		localTime->tm_min,
		localTime->tm_sec,
		localTime->tm_sec * 1000000
	);
	this->_tz = TimeZone(timestamp);
	delete localTime;
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

Date DateTime::date()
{
	return this->_date;
}

Time DateTime::time()
{
	return this->_time;
}

TimeZone DateTime::tz()
{
	return this->_tz;
}

std::string DateTime::strftime(const char *_format)
{
	auto* timeInfo = new std::tm();
	timeInfo->tm_year = this->_date.year();
	timeInfo->tm_mon = this->_date.month();
	timeInfo->tm_wday = this->_date.dayOfWeek();
	timeInfo->tm_mday = this->_date.dayOfMonth();
	timeInfo->tm_yday = this->_date.dayOfYear();
	timeInfo->tm_hour = this->_time.hour();
	timeInfo->tm_min = this->_time.minute();
	timeInfo->tm_sec = this->_time.second();
	timeInfo->tm_zone = this->_tz.getName().c_str();

	char buffer[80];

	std::strftime(buffer, 80, _format, timeInfo);

	delete timeInfo;

	return buffer;
}

DateTime DateTime::strptime(const char* _datetime, const char* _format)
{
	auto* timeInfo = new tm();
	::strptime(_datetime, _format, timeInfo);
	auto result = DateTime(
		timeInfo->tm_year,
		timeInfo->tm_mon,
		timeInfo->tm_wday,
		timeInfo->tm_mday,
		timeInfo->tm_yday,
		timeInfo->tm_hour,
		timeInfo->tm_min,
		timeInfo->tm_sec,
		timeInfo->tm_sec * 1000000,
		TimeZone(std::mktime(timeInfo))
	);
	delete timeInfo;
	return result;
}

__DATETIME_END__
