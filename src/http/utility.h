/**
 * http/utility.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: useful http helpers.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../core/signing/signer.h"


__HTTP_BEGIN__

/// Converts std::string datetime to utc epoch in seconds.
extern size_t parse_http_datetime(const std::string& http_datetime);

/// Format the time to match the RFC1123 date format as specified by HTTP
/// RFC7231 section 7.1.1.1.
///
/// `epoch_seconds` is a floating point number expressed in seconds since the
/// epoch, in UTC - such as that outputted by dt::internal::_time().
/// If set to None, it defaults to the current time.
///
/// Output a string in the format 'Wdy, DD Mon YYYY HH:MM:SS GMT'.
extern std::string http_date(size_t epoch_seconds);

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

extern core::signing::Signer get_cookie_signer(
	const std::string& secret_key,
	const std::string& salt = "xalwart.core.signing.get_cookie_signer"
);

__HTTP_END__
