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
 * utility.h
 * Purpose: some http helpers.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/regex.h"
#include "../core/string/str.h"
#include "../core/datetime/datetime.h"


__HTTP_BEGIN__

/// Converts std::string datetime to utc epoch in seconds.
extern size_t parse_http_datetime(const std::string& http_datetime);

/// Converts utc epoch seconds to std::string datetime.
extern std::string format_http_datetime(size_t epoch_seconds);

/// Writes domain and port from a given host.
///
/// Returned domain is lower-cased. If the host is invalid,
/// the domain will be empty.
void split_domain_port(
	const std::string& host, std::string& domain, std::string& port
);

/// Validate the given host for this site.
///
/// Check that the host looks valid and matches a host or host pattern in the
/// given list of `allowed_hosts`. Any pattern beginning with a period
/// matches a domain and all its sub-domains (e.g. `.example.com` matches
/// `example.com` and any sub-domain), `*` matches anything, and anything
/// else must match exactly.
///
/// Note: This function assumes that the given host is lower-cased and has
/// already had the port, if any, stripped off.
///
/// Return `true` for a valid host, `false` otherwise.
bool validate_host(
	const std::string& domain, const std::vector<std::string>& allowed_hosts
);

/// Return `true` if the host is either an exact match or a match
/// to the wildcard pattern.
///
/// Any pattern beginning with a period matches a domain and all of its
/// subdomains. (e.g. `.example.com` matches `example.com` and
/// `foo.example.com`). Anything else is an exact string match.
extern bool is_same_domain(const std::string& host, const std::string& pattern);

/// If redirecting to an absolute path (two leading slashes), a slash must be
/// escaped to prevent browsers from handling the path as schemaless and
/// redirecting to another host.
extern void escape_leading_slashes(std::string& url);

__HTTP_END__
