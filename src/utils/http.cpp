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
 * An implementation of http.h.
 */

#include "./http.h"


__UTILS_HTTP_INTERNAL_BEGIN__

core::rgx::Regex ETAG_REGEX = core::rgx::Regex(R"(((?:W\/)?"[^"]*"))");

const std::string _D = R"(<day>(\d{2}))";
const std::string _D2 = R"(<day>([ \d]\d))";
const std::string _M = R"(<mon>(\w{3}))";
const std::string _Y = R"(<year>(\d{4}))";
const std::string _Y2 = R"(<year>(\d{2}))";
const std::string _T = R"(<hour>(\d{2}):<min>(\d{2}):<sec>(\d{2}))";

core::rgx::ArgRegex RFC1123_DATE = core::rgx::ArgRegex(
	R"(\w{3}, )" + _D + R"( )" + _M + R"( )" + _Y + R"( )" + _T + R"( GMT)"
);
core::rgx::ArgRegex RFC850_DATE = core::rgx::ArgRegex(
	R"(w{6,9}, )" + _D + R"(-)" + _M + R"(-)" + _Y2 + R"( )" + _T + R"( GMT)"
);
core::rgx::ArgRegex ASCTIME_DATE = core::rgx::ArgRegex(
	R"(w{3} )" + _M + R"( )" + _D2 + R"( )" + _T + R"( )" + _Y + R"()"
);

const std::vector<std::string> MONTHS = {
	"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"
};

__UTILS_HTTP_INTERNAL_END__


__UTILS_HTTP_BEGIN__

long parse_http_date(const std::string& date)
{
	if (date.empty())
	{
		return -1;
	}

	auto rfc_1123_date = internal::RFC1123_DATE;
	auto rfc_850_date = internal::RFC850_DATE;
	auto asc_time_date = internal::ASCTIME_DATE;
	std::map<std::string, std::string> match;
	if (rfc_1123_date.search(date))
	{
		match = rfc_1123_date.groups();
	}
	else if (rfc_850_date.search(date))
	{
		match = rfc_850_date.groups();
	}
	else if (asc_time_date.search(date))
	{
		match = asc_time_date.groups();
	}
	else
	{
		return -1;
	}

	int year = std::stoi(match["year"]);
	if (year < 100)
	{
		int current_year = core::dt::gmtnow().year();
		int current_century = current_year - (current_year % 100);
		if (year - (current_year % 100) > 50)
		{
			// year that appears to be more than 50 years in the future are
			// interpreted as representing the past.
			year += current_century - 100;
		}
		else
		{
			year += current_century;
		}
	}

	int month = (int) core::utility::index_of(
		internal::MONTHS.begin(),
		internal::MONTHS.end(),
		core::str::lower(match["mon"])
	) + 1;
	int day = std::stoi(match["day"]);
	int hour = std::stoi(match["hour"]);
	int min = std::stoi(match["min"]);
	int sec = std::stoi(match["sec"]);

	auto date_time = core::dt::DateTime(year, month, day, hour, min, sec);
	return date_time.utc_epoch();
}

std::string quote_etag(const std::string& e_tag)
{
	auto e_tag_regex = internal::ETAG_REGEX;
	if (e_tag_regex.match(e_tag))
	{
		return e_tag;
	}

	return "\"" + e_tag + "\"";
}

std::vector<std::string> parse_etags(const std::string& etag_str)
{
	if (core::str::trim(etag_str) == "*")
	{
		return {"*"};
	}

	// Parse each ETag individually, and return any that are valid.
	auto etags = core::str::split(etag_str, ',');
	std::vector<std::string> result;
	auto e_tag_regex = internal::ETAG_REGEX;
	for (const auto& etag : etags)
	{
		if (e_tag_regex.search(core::str::trim(etag)))
		{
			result.push_back(e_tag_regex.group(0));
		}
	}

	return result;
}

__UTILS_HTTP_END__
