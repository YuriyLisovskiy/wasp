/**
 * http/utility.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: useful http helpers.
 */

#pragma once

// Base libraries.
#include <xalwart.base/re/regex.h>
#include <xalwart.base/re/arg_regex.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../core/signing/signer.h"


__HTTP_BEGIN__

// Converts std::string datetime to utc epoch in seconds.
extern size_t parse_http_datetime(const std::string& http_datetime);

// Format the time to match the RFC1123 date format as specified by HTTP
// RFC7231 section 7.1.1.1.
//
// `epoch_seconds` is a floating point number expressed in seconds since the
// epoch, in UTC - such as that outputted by dt::internal::_time().
// If set to None, it defaults to the current time.
//
// Output a string in the format 'Wdy, DD Mon YYYY HH:MM:SS GMT'.
extern std::string http_date(size_t epoch_seconds);

// Writes domain and port from a given host.
//
// Returned domain is lower-cased. If the host is invalid,
// the domain will be empty.
void split_domain_port(const std::string& host, std::string& domain, std::string& port);

// Validate the given host for this site.
//
// Check that the host looks valid and matches a host or host pattern in the
// given list of `allowed_hosts`. Any pattern beginning with a period
// matches a domain and all its sub-domains (e.g. `.example.com` matches
// `example.com` and any sub-domain), `*` matches anything, and anything
// else must match exactly.
//
// Note: This function assumes that the given host is lower-cased and has
// already had the port, if any, stripped off.
//
// Return `true` for a valid host, `false` otherwise.
bool validate_host(const std::string& domain, const std::vector<std::string>& allowed_hosts);

// Return `true` if the host is either an exact match or a match
// to the wildcard pattern.
//
// Any pattern beginning with a period matches a domain and all of its
// subdomains. (e.g. `.example.com` matches `example.com` and
// `foo.example.com`). Anything else is an exact string match.
extern bool is_same_domain(const std::string& host, const std::string& pattern);

// If redirecting to an absolute path (two leading slashes), a slash must be
// escaped to prevent browsers from handling the path as schemaless and
// redirecting to another host.
extern void escape_leading_slashes(std::string& url);

extern core::signing::Signer get_cookie_signer(
	const std::string& secret_key, const std::string& salt="xw::http::get_cookie_signer"
);

// TESTME: parse_http_date
// Parse a date format as specified by HTTP RFC7231 section 7.1.1.1.
//
// The three formats allowed by the RFC are accepted, even if only the first
// one is still in widespread use.
//
// Return an integer expressed in seconds since the epoch, in UTC.
extern long parse_http_date(const std::string& date);

// TESTME: quote_etag
// If the provided string is already a quoted ETag, return it.
// Otherwise, wrap the string in quotes, making it a strong ETag.
extern std::string quote_etag(const std::string& e_tag);

// TESTME: parse_etags
// Parse a string of ETags given in an If-None-Match or If-Match header as
// defined by RFC 7232. Return a std::vector<std::string> of quoted ETags,
// or {"*"} if all ETags should be matched.
extern std::vector<std::string> parse_etags(const std::string& etag_str);

__HTTP_END__


__HTTP_INTERNAL_BEGIN__

inline const re::Regex ETAG_REGEX(R"(((?:W\/)?"[^"]*"))");

inline const std::string _D = R"(<day>(\d{2}))";
inline const std::string _D2 = R"(<day>([ \d]\d))";
inline const std::string _M = R"(<mon>(\w{3}))";
inline const std::string _Y = R"(<year>(\d{4}))";
inline const std::string _Y2 = R"(<year>(\d{2}))";
inline const std::string _T = R"(<hour>(\d{2}):<min>(\d{2}):<sec>(\d{2}))";

inline const re::ArgRegex RFC1123_DATE(R"(\w{3}, )" + _D + R"( )" + _M + R"( )" + _Y + R"( )" + _T + R"( GMT)");
inline const re::ArgRegex RFC850_DATE(R"(w{6,9}, )" + _D + R"(-)" + _M + R"(-)" + _Y2 + R"( )" + _T + R"( GMT)");
inline const re::ArgRegex ASCTIME_DATE(R"(w{3} )" + _M + R"( )" + _D2 + R"( )" + _T + R"( )" + _Y + R"()");

inline const std::vector<std::string> MONTHS = {
	"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"
};

__HTTP_INTERNAL_END__
