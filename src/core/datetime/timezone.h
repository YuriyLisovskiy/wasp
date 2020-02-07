/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/datetime/timezone.h
 *
 * Purpose: utility for retrieving timezone.
 */

#pragma once

// C++ libraries.
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../regex.h"
#include "./predefined/tz_abbr_to_offset.h"
#include "./predefined/offset_to_tz_abbr.h"


__DATETIME_BEGIN__

/// A class that represents a fixed offset from the UTC.
///
/// Note: Date and time expressed according to
/// ISO 8601 currently is not supported.
class TimeZone
{
private:
	int _h_offset;
	int _min_offset;
	int _sign;

	/// Time zone abbreviation, i.e. GMT, ACT, etc.
	std::string _abbr;

	/// Time zone name, i.e. America/Vancouver, US/Central, etc.
	std::string _name;

	/// String offset, i.e. +0300
	std::string _str_offset;

	int _offset();
	void _make_from_dt(const std::string& datetime_str);
	void _make_from_offset();
	std::string _offset_to_str();
	void _parse_num_offset(const std::string& offset_str);
	void _make_default();

public:
	enum Units
	{
		HOURS,
		MINUTES,
		SECONDS,
		MICROSECONDS
	};

	explicit TimeZone(time_t when);
	explicit TimeZone(int offset);
	explicit TimeZone(
		int offset_h = 0,
		int offset_min = 0,
		const std::string& abbr = "GMT",
		const std::string& name = ""
	);
	TimeZone(const std::string& datetime);

	int offset(Units units = TimeZone::Units::SECONDS);
	std::string str_offset();
	std::string abbr();
	std::string name();
};

__DATETIME_END__


__DATETIME_INTERNAL_BEGIN__

extern rgx::Regex RE_HOURS;

extern rgx::Regex RE_ZONE_ABBR;

__DATETIME_INTERNAL_END__
