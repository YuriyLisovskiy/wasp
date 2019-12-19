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
 * timezone.h
 * Purpose: utility for retrieving timezone.
 */

#ifndef WASP_CORE_DATETIME_TIMEZONE_H
#define WASP_CORE_DATETIME_TIMEZONE_H

// C++ libraries.
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./constants.h"


__DATETIME_BEGIN__

// A class that represents a fixed offset from the UTC.
class TimeZone
{
private:
	// Offset in seconds.
	int _offset;

	// Time zone name, i.e. America/Vancouver, US/Central, etc.
	std::string _name;

public:
	enum Units
	{
		HOURS,
		MINUTES,
		SECONDS,
		MICROSECONDS
	};

	TimeZone();
	explicit TimeZone(time_t when);
	explicit TimeZone(const std::string& name);

	/// Offset parameter is time offset in seconds.
	explicit TimeZone(int offset, std::string  name);

	int get_offset(Units units);
	std::string get_name();
};

__DATETIME_END__


#endif // WASP_CORE_DATETIME_TIMEZONE_H
