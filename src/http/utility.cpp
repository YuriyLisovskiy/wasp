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
 * An implementation of http/utility.h
 */

#include "./utility.h"

// Framework modules.
#include "../core/utility.h"
#include "../core/strings.h"


__HTTP_INTERNAL_BEGIN__

core::rgx::Regex host_validation_regex = core::rgx::Regex(
	R"(([a-z0-9.-]+|\[[a-f0-9]*:[a-f0-9\.:]+\])(:\d+)?)"
);

__HTTP_INTERNAL_END__


__HTTP_BEGIN__

size_t parse_http_datetime(const std::string& http_datetime)
{
	return core::dt::Datetime::strptime(
		http_datetime, "%a, %d %b %Y %H:%M:%S GMT"
	).timestamp();
}

std::string http_date(size_t epoch_seconds)
{
	return core::utility::format_date(epoch_seconds, false, true);
}

void split_domain_port(
	const std::string& host, std::string& domain, std::string& port
)
{
	auto host_lower = core::str::lower(host);
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
		auto split = core::str::rsplit(host_lower, ':', 1);
		domain = split[0];
		if (split.size() == 2)
		{
			port = split[1];
		}

		// Remove a trailing dot (if present) from the domain.
		if (core::str::ends_with(domain, "."))
		{
			domain.pop_back();
		}
	}

	core::str::rtrim(domain, "]");
	core::str::ltrim(domain, "[");
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

	auto lc_pattern  = core::str::lower(pattern);
	return (lc_pattern[0] == '.' && (
		core::str::ends_with(host, lc_pattern) || host == lc_pattern.substr(1, lc_pattern.size() - 1)
	)) || lc_pattern == host;
}

void escape_leading_slashes(std::string& url)
{
	if (core::str::starts_with(url, "//"))
	{
		url = "/%2F" + url.substr(2);
	}
}

core::signing::Signer get_cookie_signer(const std::string& secret_key, const std::string& salt)
{
	return core::signing::Signer("" + secret_key, ':', salt);
}

__HTTP_END__
