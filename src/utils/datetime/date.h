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
 * Date definition.
 * TODO: write docs.
 */

#ifndef WASP_UTILS_DATETIME_DATE_H
#define WASP_UTILS_DATETIME_DATE_H

#include <string>

#include "../../globals.h"
#include "constants.h"


__DATETIME_BEGIN__

// An idealized naive date, assuming the current Gregorian calendar
// always was, and always will be, in effect.
//
// Properties: _year, _month, _dayOfYear and _dayOfWeek.
class Date
{
private:
	int _year;
	int _month;
	int _dayOfWeek;
	int _dayOfYear;

public:
	Date() = default;
	Date(int year, int month, int dayOfWeek, int dayOfYear);

	int year();
	int month();
	int dayOfWeek();
	int dayOfYear();

	std::string format(const char* _format);
	std::string format(const std::string& _format);
};

__DATETIME_END__


#endif // WASP_UTILS_DATETIME_DATE_H
