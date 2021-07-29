/**
 * http/utility.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./utility.h"

// Base libraries.
#include <xalwart.base/utility.h>
#include <xalwart.base/string_utils.h>


__HTTP_INTERNAL_BEGIN__

re::Regex host_validation_regex = re::Regex(
	R"(([a-z0-9.-]+|\[[a-f0-9]*:[a-f0-9\.:]+\])(:\d+)?)"
);

__HTTP_INTERNAL_END__


__HTTP_BEGIN__

size_t parse_http_datetime(const std::string& http_datetime)
{
	return (size_t)dt::Datetime::strptime(http_datetime, "%a, %d %b %Y %H:%M:%S GMT").timestamp();
}

std::string http_date(size_t epoch_seconds)
{
	return util::format_date((time_t)epoch_seconds, false, true);
}

void split_domain_port(
	const std::string& host, std::string& domain, std::string& port
)
{
	auto host_lower = str::lower(host);
	if (!internal::host_validation_regex.match(host_lower))
	{
		return;
	}

	if (host_lower.back() == ']')
	{
		domain = host_lower;
	}
	else
	{
		auto split = str::rsplit(host_lower, ':', 1);
		domain = split[0];
		if (split.size() == 2)
		{
			port = split[1];
		}

		// Remove a trailing dot (if present) from the domain.
		if (domain.ends_with("."))
		{
			domain.pop_back();
		}
	}

	domain = str::trim(domain, "[]");
}

bool validate_host(
	const std::string& host, const std::vector<std::string>& allowed_hosts
)
{
	bool result = false;
	for (const auto& pattern : allowed_hosts)
	{
		if (pattern == "*" || is_same_domain(host, pattern))
		{
			result = true;
			break;
		}
	}

	return result;
}

bool is_same_domain(const std::string& host, const std::string& pattern)
{
	if (pattern.empty())
	{
		return false;
	}

	auto lc_pattern  = str::lower(pattern);
	return (lc_pattern[0] == '.' && (
		host.ends_with(lc_pattern) || host == lc_pattern.substr(1, lc_pattern.size() - 1)
	)) || lc_pattern == host;
}

void escape_leading_slashes(std::string& url)
{
	if (url.starts_with("//"))
	{
		url = "/%2F" + url.substr(2);
	}
}

core::signing::Signer get_cookie_signer(const std::string& secret_key, const std::string& salt)
{
	return core::signing::Signer("" + secret_key, ':', salt);
}

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
		match = rfc_1123_date.args();
	}
	else if (rfc_850_date.search(date))
	{
		match = rfc_850_date.args();
	}
	else if (asc_time_date.search(date))
	{
		match = asc_time_date.args();
	}
	else
	{
		return -1;
	}

	int year = std::stoi(match["year"]);
	if (year < 100)
	{
		int current_year = dt::Datetime::utc_now().date().year();
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

	int month = (int) util::index_of(
			str::lower(match["mon"]),
			internal::MONTHS.begin(),
			internal::MONTHS.end()
	) + 1;
	int day = std::stoi(match["day"]);
	int hour = std::stoi(match["hour"]);
	int min = std::stoi(match["min"]);
	int sec = std::stoi(match["sec"]);

	auto date_time = dt::Datetime(year, month, day, hour, min, sec);
	return (long)date_time.timestamp();
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
	if (str::trim(etag_str) == "*")
	{
		return {"*"};
	}

	// Parse each ETag individually, and return any that are valid.
	auto etags = str::split(etag_str, ',');
	std::vector<std::string> result;
	auto e_tag_regex = internal::ETAG_REGEX;
	for (const auto& etag : etags)
	{
		if (e_tag_regex.search(str::trim(etag)))
		{
			result.push_back(e_tag_regex.group(1));
		}
	}

	return result;
}

__HTTP_END__
