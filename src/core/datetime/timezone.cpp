#include <utility>

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
 * An implementation of timezone.h.
 */

#include "./timezone.h"


__DATETIME_BEGIN__

TimeZone::TimeZone() : _name("UTC"), _offset(0)
{
}

TimeZone::TimeZone(time_t when) : _name("")
{
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	int h = std::stoi(s.substr(0, 3), nullptr, 10);
	int m = std::stoi(s[0] + s.substr(3), nullptr, 10);
	this->_offset = h * 3600 + m * 60;
	this->tz_from_offset(this->_offset);
}

TimeZone::TimeZone(size_t offset)
{
	this->_offset = offset;
	this->tz_from_offset(this->_offset);
}

TimeZone::TimeZone(int offset, std::string name) : _offset(offset), _name(std::move(name))
{
}

TimeZone::TimeZone(const std::string& name) : _name(name)
{
	if (internal::TZ_TO_OFFSET.contains(name))
	{
		this->_offset = internal::TZ_TO_OFFSET.get(name);
	}
	else
	{
		this->_offset = internal::TZ_ABBR_TO_OFFSET.get(name, 0);
	}

	if (this->_offset == 0)
	{
		this->_name = "GMT";
	}
}

int TimeZone::get_offset(Units units)
{
	int result;
	switch (units)
	{
		case TimeZone::Units::HOURS:
			result = this->_offset / 3600;
			break;
		case TimeZone::Units::MINUTES:
			result = this->_offset / 60;
			break;
		case TimeZone::Units::SECONDS:
			result = this->_offset;
			break;
		case TimeZone::Units::MICROSECONDS:
			result = this->_offset * 1000000;
			break;
		default:
			result = this->_offset;
			break;
	}

	return result;
}

std::string TimeZone::get_name()
{
	return this->_name;
}

void TimeZone::tz_from_offset(size_t offset)
{
	this->_name = internal::OFFSET_TO_TZ_ABBR.get(offset, "");
	if (this->_name.empty())
	{
		this->_offset = 0;
		this->_name = "GMT";
	}
}

__DATETIME_END__
