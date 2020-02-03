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


__DATETIME_INTERNAL_BEGIN__

rgx::Regex RE_HOURS = rgx::Regex(R"(([+-](?:2[0-3]|[01][0-9])[0-5][0-9]))");

rgx::Regex RE_ZONE_ABBR = rgx::Regex(R"((\b[A-Z]+\b))");

__DATETIME_INTERNAL_END__


__DATETIME_BEGIN__

TimeZone::TimeZone(time_t when)
{
	auto const tm = *std::localtime(&when);
	std::ostringstream os;
	os << std::put_time(&tm, "%z");
	std::string s = os.str();
	auto offset = std::stoi(s.substr(0, 3), nullptr, 10);
	this->_sign = offset < 0 ? -1 : 1;
	this->_h_offset = std::abs(offset);
	this->_min_offset = std::stoi(s.substr(3), nullptr, 10);

	this->_make_from_offset();
}

TimeZone::TimeZone(int offset)
{
	double hm = offset / 3600.0;
	this->_sign = hm < 0 ? -1 : 1;
	this->_h_offset = std::abs((int)hm);
	this->_min_offset = std::abs((int)(std::abs(hm) - (this->_h_offset * 60)));
	this->_make_from_offset();
}

TimeZone::TimeZone(
	int offset_h,
	int offset_min,
	const std::string& abbr,
	const std::string& name
)
{
	this->_abbr = abbr;
	if (name.empty())
	{
		this->_name = abbr;
	}
	else
	{
		this->_name = name;
	}

	this->_sign = offset_h < 0 ? -1 : 1;
	this->_h_offset = std::abs(offset_h);
	this->_min_offset = std::abs(offset_min);
	this->_str_offset = this->_offset_to_str();
}

TimeZone::TimeZone(const std::string& datetime)
{
	this->_make_from_dt(datetime);
}

int TimeZone::offset(Units units)
{
	int result;
	int offset = this->_offset();
	switch (units)
	{
		case TimeZone::Units::HOURS:
			result = offset / 3600;
			break;
		case TimeZone::Units::MINUTES:
			result = offset / 60;
			break;
		case TimeZone::Units::SECONDS:
			result = offset;
			break;
		case TimeZone::Units::MICROSECONDS:
			result = offset * 1000000;
			break;
		default:
			result = offset;
			break;
	}

	return result;
}

std::string TimeZone::str_offset()
{
	return this->_str_offset;
}

std::string TimeZone::abbr()
{
	return this->_abbr;
}

std::string TimeZone::name()
{
	return this->_name;
}

int TimeZone::_offset()
{
	return this->_sign * (this->_h_offset + this->_min_offset);
}

void TimeZone::_make_from_offset()
{
	this->_str_offset = this->_offset_to_str();
	this->_abbr = internal::OFFSET_TO_TZ_ABBR.get(this->_str_offset, "");
	if (this->_abbr.empty())
	{
		this->_make_default();
	}
	else
	{
		this->_name = this->_abbr;
	}
}

void TimeZone::_make_from_dt(const std::string& datetime_str)
{
	auto re_hours = internal::RE_HOURS;
	auto re_zone_abbr = internal::RE_ZONE_ABBR;
	if (re_hours.search(datetime_str))
	{
		auto offset = re_hours.group(0);
		this->_parse_num_offset(offset);
		this->_abbr = internal::OFFSET_TO_TZ_ABBR.get(offset, "");
		this->_name = this->_abbr;
	}
	else if (re_zone_abbr.search(datetime_str))
	{
		this->_abbr = re_zone_abbr.group(0);
		this->_parse_num_offset(
			internal::TZ_ABBR_TO_OFFSET.get(this->_abbr, "")
		);
		this->_name = this->_abbr;
	}
	else
	{
		this->_make_default();
	}

	this->_str_offset = this->_offset_to_str();
}

std::string TimeZone::_offset_to_str()
{
	std::string h;
	if (this->_h_offset < 10)
	{
		h += "0";
	}

	h += std::to_string(std::abs(this->_h_offset));
	std::string min = "00";
	if (this->_min_offset != 0)
	{
		min = std::to_string(this->_min_offset);
	}

	return (this->_sign < 0 ? "-" : "+") + h + min;
}

void TimeZone::_parse_num_offset(const std::string& offset_str)
{
	auto h_offset = std::stoi(
		offset_str.substr(0, 3), nullptr, 10
	);
	this->_sign = h_offset < 0 ? -1 : 1;
	this->_h_offset = std::abs(h_offset);
	this->_min_offset = std::stoi(
		offset_str.substr(3), nullptr, 10
	);
}

void TimeZone::_make_default()
{
	this->_abbr = "GMT";
	this->_name = this->_abbr;
	this->_h_offset = 0;
	this->_min_offset = 0;
	this->_sign = 1;
	this->_str_offset = "+0000";
}

__DATETIME_END__
