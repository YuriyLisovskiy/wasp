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
 * An implementation of datetime.h.
 */

#include "./datetime.h"


__DATETIME_INTERNAL_BEGIN__

time_t now()
{
	return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
}

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

DateTime now()
{
	auto time_now = internal::now();
	auto local_time = std::localtime(&time_now);
	return DateTime(
		local_time->tm_year + 1900,
		local_time->tm_mon + 1,
		local_time->tm_wday,
		local_time->tm_mday,
		local_time->tm_yday,
		local_time->tm_hour,
		local_time->tm_min,
		local_time->tm_sec,
		0,
		TimeZone(std::string(local_time->tm_zone))
	);
}

DateTime gmtnow()
{
	auto time_now = internal::now();
	auto gm_time = std::gmtime(&time_now);
	return DateTime(
		gm_time->tm_year + 1900,
		gm_time->tm_mon + 1,
		gm_time->tm_wday,
		gm_time->tm_mday,
		gm_time->tm_yday,
		gm_time->tm_hour,
		gm_time->tm_min,
		gm_time->tm_sec,
		0,
		TimeZone()
	);
}

/// DateTime class implementation.
DateTime::DateTime() : DateTime(internal::now())
{
}

DateTime::DateTime(int year, int month, int day, int hour, int minute, int second)
	: _date(year, month, day), _time(hour, minute, second, 0), _tz()
{
}

DateTime::DateTime(
	int year, int month, int day_of_week, int day_of_month, int day_of_year,
	int hour, int minute, int second, int microsecond,
	TimeZone tz
) :
	_date(year, month, day_of_week, day_of_month, day_of_year),
	_time(hour, minute, second, microsecond),
	_tz(std::move(tz))
{
}

DateTime::DateTime(Date date, Time time, TimeZone tz)
	: _date(date), _time(time), _tz(std::move(tz))
{
}

DateTime::DateTime(time_t timestamp)
{
	auto local_time = std::localtime(&timestamp);
	this->_date = Date(
		local_time->tm_year + 1900,
		local_time->tm_mon + 1,
		local_time->tm_wday,
		local_time->tm_mday,
		local_time->tm_yday
	);
	this->_time = Time(
		local_time->tm_hour,
		local_time->tm_min,
		local_time->tm_sec,
		0
	);
	this->_tz = TimeZone(timestamp);
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

size_t DateTime::timestamp()
{
	auto* time_info = this->_to_std_tm();
	time_info->tm_isdst = 1;
	time_t ts = ::timegm(time_info);
	delete time_info;
	return ts;
}

std::string DateTime::strftime(const char* _format)
{
	auto* time_info = this->_to_std_tm();
	char buffer[80];
	std::strftime(buffer, 80, this->strf_tz(_format).c_str(), time_info);
	delete time_info;
	return buffer;
}

DateTime DateTime::strptime(const char* _datetime, const char* _format)
{
	struct tm time_info{};
	::strptime(_datetime, _format, &time_info);
	auto result = DateTime(
		time_info.tm_year + 1900,
		time_info.tm_mon + 1,
		time_info.tm_wday,
		time_info.tm_mday,
		time_info.tm_yday,
		time_info.tm_hour,
		time_info.tm_min,
		time_info.tm_sec,
		0,
		TimeZone(_datetime)
	);
	return result;
}

std::tm* DateTime::_to_std_tm()
{
	auto* time_info = new std::tm();
	time_info->tm_year = this->_date.year() - 1900;
	time_info->tm_mon = this->_date.month() - 1;
	time_info->tm_wday = this->_date.day_of_week();
	time_info->tm_mday = this->_date.day_of_month();
	time_info->tm_yday = this->_date.day_of_year();
	time_info->tm_hour = this->_time.hour();
	time_info->tm_min = this->_time.minute();
	time_info->tm_sec = this->_time.second();
	time_info->tm_zone = this->_tz.abbr().c_str();
	time_info->tm_gmtoff = this->_tz.offset();
	return time_info;
}

std::string DateTime::strf_tz(const char* _format)
{
	auto s_format = std::string(_format);
	size_t pos;
	pos = s_format.find("%Z");
	if (pos != std::string::npos)
	{
		s_format.replace(pos, pos + 1, this->_tz.abbr());
	}
	else
	{
		pos = s_format.find("%z");
		if (pos != std::string::npos)
		{
			s_format.replace(pos, pos + 1, this->_tz.str_offset());
		}
	}

	return s_format;
}

__DATETIME_END__
