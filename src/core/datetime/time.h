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
 * time.h
 * Purpose: utility for working with time.
 */

#ifndef WASP_CORE_DATETIME_TIME_H
#define WASP_CORE_DATETIME_TIME_H

// C++ libraries.
#include <string>
#include <chrono>

// Module definitions.
#include "./_def_.h"


__DATETIME_BEGIN__

/// An idealized time, independent of any particular day, assuming that
/// every day has exactly 24*60*60 seconds (there is no notion of “leap seconds” here).
///
/// Properties: _hour, _minute, _second and _microsecond.
class Time
{
private:
	int _hour;
	int _minute;
	int _second;
	int _microsecond;

public:
	Time();
	Time(int hour, int minute, int second, int microsecond);

	int hour();
	int minute();
	int second();
	int microsecond();
};


template <typename _TimeT = std::chrono::milliseconds>
class Measure
{
private:
	std::chrono::high_resolution_clock::time_point _begin;
	std::chrono::high_resolution_clock::time_point _end;

public:
	void start()
	{
		this->_begin = std::chrono::high_resolution_clock::now();
	}

	void end()
	{
		this->_end = std::chrono::high_resolution_clock::now();
	}

	double elapsed(bool reset = true)
	{
		auto result = std::chrono::duration_cast<_TimeT>(this->_end - this->_begin).count();
		if (reset)
		{
			this->reset();
		}

		return result;
	}

	void reset()
	{
		this->_begin = {};
		this->_end = {};
	}
};

__DATETIME_END__


#endif // WASP_CORE_DATETIME_TIME_H
