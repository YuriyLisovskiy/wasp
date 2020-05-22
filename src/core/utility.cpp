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
 * An implementation of core/utility.h
 */

#include "./utility.h"

// C++ libraries.
#include <cxxabi.h>
#include <cassert>


__UTILITY_BEGIN__

// Statuses:
// 0 - operation succeeded
// 1 - a memory allocation failure occurred
// 2 - mangled_name is not a valid name under the C++ ABI mangling rules
// 3 - one of the arguments is invalid
std::string demangle(const char* name)
{
	int status = -4;
	std::unique_ptr<char, void(*)(void*)> res {
		abi::__cxa_demangle(name, nullptr, nullptr, &status),
		std::free
	};

	return status == 0 ? res.get() : name;
}

std::string format_datetime(
	const dt::Datetime* dt, bool use_gmt
)
{
	assert(dt != nullptr);
	auto now = dt->time_tuple();
	std::string zone;
	if (use_gmt)
	{
		if (!dt->tz_info() || *dt->tz_info() != dt::Timezone::UTC)
		{
			throw std::invalid_argument("use_gmt option requires a UTC datetime");
		}

		zone = "GMT";
	}
	else if (!dt->tz_info())
	{
		zone = "-0000";
	}
	else
	{
		zone = dt->strftime("%z");
	}

	return internal::_format_timetuple_and_zone(&now, zone);
}

std::string format_date(
	time_t time_val, bool local_time, bool use_gmt
)
{
	// Note: we cannot use strftime() because that honors the locale and RFC
	// 2822 requires that day and month names be the English abbreviations.
	if (!time_val)
	{
		time_val = dt::internal::_time();
	}

	std::shared_ptr<dt::Datetime> dt;
	if (local_time || use_gmt)
	{
		dt = std::make_shared<dt::Datetime>(
			dt::Datetime::from_timestamp(
				time_val, std::make_shared<dt::Timezone>(dt::Timezone::UTC)
			)
		);
	}
	else
	{
		dt = std::make_shared<dt::Datetime>(
			dt::Datetime::utc_from_timestamp(time_val)
		);
	}

	if (local_time)
	{
		*dt = dt->as_timezone();
		use_gmt = false;
	}

	return format_datetime(dt.get(), use_gmt);
}

__UTILITY_END__


__UTILITY_INTERNAL_BEGIN__

std::string _format_timetuple_and_zone(
	dt::tm_tuple* time_tuple, const std::string& zone
)
{
	return dt::internal::_DAY_NAMES[((time_tuple->tm_wday+6)%7)+1] + ", " +
		dt::internal::_lf(time_tuple->tm_mday) + " " +
		dt::internal::_MONTH_NAMES[time_tuple->tm_mon+1] + " " +
		dt::internal::_lf(time_tuple->tm_year, 4) + " " +
		dt::internal::_lf(time_tuple->tm_hour) + ":" +
		dt::internal::_lf(time_tuple->tm_min) + ":" +
		dt::internal::_lf(time_tuple->tm_sec) + " " +
		zone;
}

__UTILITY_INTERNAL_END__
