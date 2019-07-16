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
 * timezone definition.
 * TODO: write docs.
 */

#ifndef WASP_UTILS_DATETIME_TIMEZONE_H
#define WASP_UTILS_DATETIME_TIMEZONE_H

#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

#include "../../globals.h"


__DATETIME_BEGIN__

// A class that represents a fixed offset from the UTC.
class TimeZone
{
private:
	// Offset in seconds.
	int _offset;

	// Time zone name
	std::string _name;

public:
	enum Units
	{
		HOURS,
		MINUTES,
		SECONDS,
		MICROSECONDS
	};

	TimeZone() = default;
	explicit TimeZone(time_t when);
	explicit TimeZone(const std::string& name);

	// offset parameter is time offset in seconds.
	explicit TimeZone(int offset);

	int getOffset(Units units);
	std::string getName();
};

__DATETIME_END__


#endif // WASP_UTILS_DATETIME_TIMEZONE_H