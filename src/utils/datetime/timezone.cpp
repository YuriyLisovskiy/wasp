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
 * timezone implementation.
 * TODO: write docs.
 */

#include "timezone.h"


__DATETIME_BEGIN__

TimeZone::TimeZone(time_t when)
{
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	int h = std::stoi(s.substr(0, 3), nullptr, 10);
	int m = std::stoi(s[0] + s.substr(3), nullptr, 10);

	this->_offset = h * 3600 + m * 60;

	// TODO: init this->_name by offset
}

TimeZone::TimeZone(int offset)
{
	this->_offset = offset;

	// TODO: init this->_name by offset
}

TimeZone::TimeZone(const std::string& name)
{
	// TODO: init this->_offset by name

	this->_name = name;
}

int TimeZone::getOffset(Units units = Units::SECONDS)
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

std::string TimeZone::getName()
{
	return this->_name;
}

__DATETIME_END__
