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
 * date.h
 * Purpose: utility for working with date.
 */

#pragma once

// C++ libraries.
#include <ctime>
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./constants.h"


__DATETIME_BEGIN__

/// An idealized naive date, assuming the current Gregorian calendar
/// always was, and always will be, in effect.
///
/// Properties: _year, _month, _day_of_year and _day_of_week.
class Date
{
private:
	int _year;
	int _month;
	int _day_of_week;
	int _day_of_month;
	int _day_of_year;

	void _init(
		int year, int month, int day_of_week, int day_of_month, int day_of_year
	);

public:
	Date();
	Date(int year, int month, int day);
	Date(int year, int month, int day_of_week, int day_of_month, int day_of_year);

	int year();
	int month();
	int day_of_week();
	int day_of_month();
	int day_of_year();
};

__DATETIME_END__
