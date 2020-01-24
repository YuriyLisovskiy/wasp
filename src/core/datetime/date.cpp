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
 * An implementation of date.h.
 */

#include "./date.h"


__DATETIME_BEGIN__

void Date::_init(int year, int month, int day_of_week, int day_of_month, int day_of_year)
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
	this->_day_of_week = day_of_week;
	this->_day_of_month = day_of_month;
	this->_day_of_year = day_of_year;
}

Date::Date() : _year(1970), _month(1), _day_of_week(1), _day_of_month(1), _day_of_year(1)
{
}

Date::Date(int year, int month, int day)
{
	std::tm time_info {0, 0, 0, day, month, year - 1900};
	time_info.tm_zone = "GMT";
	std::time_t c_time = std::mktime(&time_info);
	const std::tm* time_out = std::localtime(&c_time);
	this->_init(year, month, time_out->tm_wday, day, time_out->tm_yday);
}

Date::Date(int year, int month, int day_of_week, int day_of_month, int day_of_year)
{
	this->_init(year, month, day_of_week, day_of_month, day_of_year);
}

int Date::year()
{
	return this->_year;
}

int Date::month()
{
	return this->_month;
}

int Date::day_of_week()
{
	return this->_day_of_week;
}

int Date::day_of_month()
{
	return this->_day_of_month;
}

int Date::day_of_year()
{
	return this->_day_of_year;
}

__DATETIME_END__
