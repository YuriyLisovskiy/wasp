/**
 * http/utility.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./utility.h"

// Core libraries.
#include <xalwart.core/utility.h>
#include <xalwart.core/string_utils.h>


__HTTP_INTERNAL_BEGIN__

rgx::Regex host_validation_regex = rgx::Regex(
	R"(([a-z0-9.-]+|\[[a-f0-9]*:[a-f0-9\.:]+\])(:\d+)?)"
);

__HTTP_INTERNAL_END__


__HTTP_BEGIN__

size_t parse_http_datetime(const std::string& http_datetime)
{
	return dt::Datetime::strptime(
		http_datetime, "%a, %d %b %Y %H:%M:%S GMT"
	).timestamp();
}

std::string http_date(size_t epoch_seconds)
{
	return utility::format_date(epoch_seconds, false, true);
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

	str::rtrim(domain, "]");
	str::ltrim(domain, "[");
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

__HTTP_END__
