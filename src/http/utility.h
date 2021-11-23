/**
 * http/utility.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Http helpers.
 */

#pragma once

// Base libraries.
#include <xalwart.base/re/regex.h>
#include <xalwart.base/re/arg_regex.h>
#include <xalwart.base/utility.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "signer.h"


__HTTP_BEGIN__

inline const short token_table_size = 77;
inline const char token_table[token_table_size] = {
	'!', '#', '$', '%', '&', '\'', '*', '+', '-', '.',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'^', '_', '`',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'|', '~'
};

// TESTME: is_token_byte
inline bool is_token_byte(char b)
{
	return b < 127 && std::binary_search(token_table, token_table + token_table_size, b);
}

// TESTME: is_not_token
inline bool is_not_token(char b)
{
	return !is_token_byte(b);
}

// TESTME: parse_http_datetime
// Converts std::string datetime to utc epoch in seconds.
inline size_t parse_http_datetime(const std::string& http_datetime)
{
	return (size_t)dt::Datetime::strptime(http_datetime, "%a, %d %b %Y %H:%M:%S GMT").timestamp();
}

// TESTME: http_date
// Format the time to match the RFC1123 date format as specified by HTTP
// RFC7231 section 7.1.1.1.
//
// `epoch_seconds` is a floating point number expressed in seconds since the
// epoch, in UTC - such as that outputted by dt::internal::_time().
// If set to None, it defaults to the current time.
//
// Output a string in the format 'Wdy, DD Mon YYYY HH:MM:SS GMT'.
inline std::string http_date(size_t epoch_seconds)
{
	return util::format_date((time_t)epoch_seconds, false, true);
}

// TESTME: split_domain_port
// Writes domain and port from a given host.
//
// Returned domain is lower-cased. If the host is invalid,
// the domain will be empty.
void split_domain_port(const std::string& host, std::string& domain, std::string& port);

// TESTME: validate_host
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

// TESTME: is_same_domain
// Return `true` if the host is either an exact match or a match
// to the wildcard pattern.
//
// Any pattern beginning with a period matches a domain and all of its
// subdomains. (e.g. `.example.com` matches `example.com` and
// `foo.example.com`). Anything else is an exact string match.
extern bool is_same_domain(const std::string& host, const std::string& pattern);

// TESTME: escape_leading_slashes
// If redirecting to an absolute path (two leading slashes), a slash must be
// escaped to prevent browsers from handling the path as schemaless and
// redirecting to another host.
extern void escape_leading_slashes(std::string& url);

// TESTME: get_cookie_signer
// TODO: docs for 'get_cookie_signer'
inline Signer get_cookie_signer(
	const std::string& secret_key, const std::string& salt="xw::http::get_cookie_signer"
)
{
	return Signer("" + secret_key, ':', salt);
}

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
