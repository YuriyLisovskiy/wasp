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
 * An implementation of time.h.
 */

#include "time.h"


__DATETIME_BEGIN__

// 'Time' class implementation.
Time::Time() : _hour(0), _minute(0), _second(0), _microsecond(0)
{
}

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

__DATETIME_END__
