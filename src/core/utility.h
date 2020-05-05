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
 * core/utility.h
 *
 * Purpose: core utilities.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./datetime.h"


__UTILITY_BEGIN__

template <typename ItemT>
bool contains(const ItemT& to_check, const std::vector<ItemT>& items)
{
	for (const auto& item : items)
	{
		if (item == to_check)
		{
			return true;
		}
	}

	return false;
}

template <typename ItemT, typename IteratorT>
long index_of(IteratorT begin, IteratorT end, const ItemT& item)
{
	auto it = std::find(begin, end, item);
	if (it == end)
	{
		return -1;
	}

	return std::distance(begin, it);
}

/// Converts typeid.name() to full name.
extern std::string demangle(const char* name);

// Turn a datetime into a date string as specified in RFC 2822.
//
// If usegmt is True, dt must be an aware datetime with an offset of zero.  In
// this case 'GMT' will be rendered instead of the normal +0000 required by
// RFC2822.  This is to support HTTP headers involving date stamps.
extern std::string format_datetime(
	const dt::Datetime* dt, bool use_gmt = false
);

// Returns a date string as specified by RFC 2822, e.g.:
//
// Fri, 09 Nov 2001 01:08:47 -0000
//
// Optional timeval if given is a floating point time value as accepted by
// gmtime() and localtime(), otherwise the current time is used.
//
// Optional localtime is a flag that when True, interprets timeval, and
// returns a date relative to the local timezone instead of UTC, properly
// taking daylight savings time into account.
//
// Optional argument usegmt means that the timezone is written out as
// an ascii string, not numeric one (so "GMT" instead of "+0000"). This
// is needed for HTTP, and is only used when localtime==false.
extern std::string format_date(
	time_t time_val, bool local_time = false, bool use_gmt = false
);

__UTILITY_END__


__UTILITY_INTERNAL_BEGIN__

extern std::string _format_timetuple_and_zone(
	tm* tm_tuple, const std::string& zone
);

__UTILITY_INTERNAL_END__
