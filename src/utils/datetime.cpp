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


__DATETIME_BEGIN__

// Measure class implementation.
template<typename _TimeT>
void Measure<_TimeT>::start()
{
	this->_begin = std::chrono::high_resolution_clock::now();
}

template<typename _TimeT>
void Measure<_TimeT>::end()
{
	this->_end = std::chrono::high_resolution_clock::now();
}

template<typename _TimeT>
double Measure<_TimeT>::elapsed(bool reset)
{
	auto result = std::chrono::duration_cast<_TimeT>(this->_end - this->_begin).count();
	if (reset)
	{
		this->reset();
	}
	return result;
}

template<typename _TimeT>
void Measure<_TimeT>::reset()
{
	this->_begin = 0;
	this->_end = 0;
}

time_t nowAsTimeT()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

DateTime now()
{
	auto timeNow = nowAsTimeT();
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
	auto timeNow = nowAsTimeT();
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


// TimeZone class implementation.
TimeZone::TimeZone(time_t when)
{
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	int h = std::stoi(s.substr(0, 3), nullptr, 10);
	int m = std::stoi(s[0] + s.substr(3), nullptr, 10);

	this->_seconds = h * 3600 + m * 60;
}

int TimeZone::hours()
{
	return this->_seconds / 3600;
}

int TimeZone::minutes()
{
	return this->_seconds / 60;
}

int TimeZone::seconds()
{
	return this->_seconds;
}

int TimeZone::microseconds()
{
	return this->_seconds * 1000000;
}

TimeZone::TimeZone(int offset)
{
	this->_seconds = offset;
}

int TimeZone::getOffset(OffsetAs offsetAs = OffsetAs::SECONDS)
{
	int result;
	switch (offsetAs)
	{
		case OffsetAs::HOURS:
			result = this->_seconds / 3600;
			break;
		case OffsetAs::MINUTES:
			result = this->_seconds / 60;
			break;
		case OffsetAs::SECONDS:
			result = this->_seconds;
			break;
		case OffsetAs::MICROSECONDS:
			result = this->_seconds * 1000000;
			break;
		default:
			result = this->_seconds;
			break;
	}
	return result;
}


// Date class implementation.
Date::Date(int year, int month, int dayOfWeek, int dayOfYear)
{
	if (year < MIN_YEAR)
	{
		year = MIN_YEAR;
	}
	else if (year > MAX_YEAR)
	{
		year = MAX_YEAR;
	}

	this->_year = year;
	this->_month = month;
	this->_dayOfWeek = dayOfWeek;
	this->_dayOfYear = dayOfYear;
}

int Date::year()
{
	return this->_year;
}

int Date::month()
{
	return this->_month;
}

int Date::dayOfWeek()
{
	return this->_dayOfWeek;
}

int Date::dayOfYear()
{
	return this->_dayOfYear;
}


// Time class implementation.
Time::Time(int hour, int minute, int second, int microsecond)
{
	this->_hour = hour;
	this->_minute = minute;
	this->_second = second;
	this->_microsecond = microsecond;
}

int Time::hour()
{
	return this->_hour;
}

int Time::minute()
{
	return this->_minute;
}

int Time::second()
{
	return this->_second;
}

int Time::microsecond()
{
	return this->_microsecond;
}


// DateTime class implementation.
DateTime::DateTime(int year, int month, int dayOfWeek, int dayOfYear, int hour, int minute, int second, int microsecond, TimeZone tz)
{
	this->_date = Date(year, month, dayOfWeek, dayOfYear);
	this->_time = Time(hour, minute, second, microsecond);
	this->_tz = tz;
}

__DATETIME_END__
