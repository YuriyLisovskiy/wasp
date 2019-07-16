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
 * Date implementation.
 * TODO: write docs.
 */

#include "date.h"


__DATETIME_BEGIN__

Date::Date() : _year(1970), _month(1), _dayOfWeek(1), _dayOfMonth(1), _dayOfYear(1)
{
}

Date::Date(int year, int month, int dayOfWeek, int dayOfMonth, int dayOfYear)
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
	this->_dayOfMonth = dayOfMonth;
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

int Date::dayOfMonth()
{
	return this->_dayOfMonth;
}

int Date::dayOfYear()
{
	return this->_dayOfYear;
}

__DATETIME_END__
