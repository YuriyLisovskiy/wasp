/**
 * http/url.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Parses URLs and implements query escaping.
 *
 * See RFC 3986. This package generally follows RFC 3986, except where
 * it deviates for compatibility reasons. When sending changes, first
 * search old issues for history on decisions. Unit tests should also
 * contain references to issue numbers with details.
 *
 * Implementation is based on Golang 1.15.8 linux/amd64:
 * https://github.com/golang/go/blob/master/src/net/url/url.go
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/collections/multi_dictionary.h>

// Module definitions.
#include "./_def_.h"


__HTTP_INTERNAL_BEGIN__

inline constexpr char UPPER_HEX[] = "0123456789ABCDEF";

// TESTME: valid_optional_port
extern bool valid_optional_port(const std::string& port);

// TESTME: split_host_port
extern std::pair<std::string, std::string> split_host_port(const std::string& host_port);

enum class EscapeMode
{
	EncodeHost, EncodePath, EncodeFragment, EncodeUserPassword,
	EncodeQueryComponent, EncodeZone, EncodePathSegment
};

// TESTME: should_escape
// Return true if the specified character should be escaped when
// appearing in a URL string, according to RFC 3986.
extern bool should_escape(char c, EscapeMode mode);

// TESTME: escape
extern std::string escape(const std::string& s, EscapeMode mode);

// TESTME: is_hex
extern bool is_hex(char c);

// TESTME: unhex
extern char unhex(char c);

// TESTME: unescape
// Unescapes a string; the mode specifies
// which section of the URL string is being unescaped.
extern std::string unescape(std::string s, EscapeMode mode);

// TESTME: valid_encoded
// Checks whether 's' is a valid encoded path or fragment,
// according to mode. It must not contain any bytes that
// require escaping during encoding.
extern bool valid_encoded(const std::string& s, EscapeMode mode);

// TESTME: split
// Splits 's' into two substrings separated by the first occurrence of
// 'sep'. If 'cut_sep' is true then 'sep' is excluded from the second substring.
// If 'sep' does not occur in 's' then 's' and the empty string is returned.
extern std::pair<std::string, std::string> split(const std::string& s, char sep, bool cut_sep=true);

// TESTME: get_scheme
extern std::pair<std::string, std::string> get_scheme(const std::string& raw_url);

// TESTME: string_contains_ctl_byte
// Reports whether s contains any ASCII control character.
inline bool string_contains_ctl_byte(const std::string& s)
{
	return std::any_of(s.begin(), s.end(), [](const char& b) -> bool { return b < ' ' || b == 0x7f; });
}

// TESTME: parse_host
// Parses host as an authority without user information.
extern std::string parse_host(std::string host);

// TESTME: valid_user_info
// Reports whether s is a valid userinfo string per RFC 3986 Section 3.2.1.
extern bool valid_user_info(const std::string& s);

// TESTME: resolve_path
// Applies special path segments from refs and applies
// them to base, per RFC 3986.
extern std::string resolve_path(const std::string& base, const std::string& ref);

__HTTP_INTERNAL_END__


__HTTP_BEGIN__

struct Query : public collections::MultiDictionary<std::string, std::string>
{
	// TESTME: encode
	// Encodes the values into ``URL encoded'' form
	// ("bar=baz&foo=quux") sorted by key.
	[[nodiscard]]
	std::string encode() const;
};

// TESTME: query_escape
// 'query_escape' escapes the string so it can be safely placed
// inside a URL query.
inline std::string query_escape(const std::string& s)
{
	return internal::escape(s, internal::EscapeMode::EncodeQueryComponent);
}

// TESTME: query_unescape
// 'query_unescape' does the inverse transformation of 'query_escape',
// converting each 3-byte encoded substring of the form "%AB" into the
// hex-decoded byte 0xAB.
// It throws an error if any % is not followed by two hexadecimal
// digits.
inline std::string query_unescape(const std::string& s)
{
	return internal::unescape(s, internal::EscapeMode::EncodeQueryComponent);
}

// TESTME: parse_query
// 'parse_query' parses the URL-encoded query string and returns
// a 'collections::MultiDictionary' listing the values specified
// for each key.
// 'parse_query' always returns a 'collections::MultiDictionary'
// containing all the valid query parameters found.
//
// Query is expected to be a list of key=value settings separated by
// ampersands or semicolons. A setting without an equals sign is
// interpreted as a key set to an empty value.
extern Query parse_query(std::string query);

// TESTME: Url
struct URL final
{
	struct UserInfo
	{
		std::string username;
		std::string password;

		inline explicit operator bool() const
		{
			return !this->username.empty();
		}

		inline bool operator !() const
		{
			return !this->operator bool();
		}

		[[nodiscard]]
		inline std::string str() const
		{
			auto s = internal::escape(this->username, internal::EscapeMode::EncodeUserPassword);
			if (!this->password.empty())
			{
				s += ":" + internal::escape(this->password, internal::EscapeMode::EncodeUserPassword);
			}

			return s;
		}
	};

	std::string scheme;
	std::string opaque;
	UserInfo user;
	std::string host;
	std::string path;
	std::string raw_path;
	bool force_query;
	std::string raw_query;
	std::string fragment;
	std::string raw_fragment;

	// Returns 'host' without any valid port if it is present.
	//
	// If result is enclosed in square brackets they will be removed
	// from the result.
	[[nodiscard]]
	inline std::string hostname() const
	{
		auto [host_name, _] = internal::split_host_port(this->host);
		return host_name;
	}

	// Returns the port number without the leading colon.
	//
	// If 'host' does not contain a valid port number, an
	// empty string will be returned.
	[[nodiscard]]
	inline std::string port() const
	{
		auto [_, port] = internal::split_host_port(this->host);
		return port;
	}

	// Returns whether the url is absolute.
	// Absolute means that the url has not empty scheme.
	[[nodiscard]]
	inline bool is_abs() const
	{
		return !this->scheme.empty();
	}

	// Sets the 'path' and 'raw_path' fields of the URL based on provided
	// escaped path 'p'. 'raw_path' is only specified when it differs from
	// the default encoding of the path.
	// For example:
	// - set_path("/foo/bar") will set path="/foo/bar" and raw_path=""
	// - set_path("/foo%2fbar") will set path="/foo/bar" and raw_path="/foo%2fbar"
	void set_path(const std::string& p);

	// Returns the escaped path from 'path'.
	// Returns 'raw_path' when it is a valid escaping of 'path'.
	// Otherwise ignores 'raw_path' and computes an escaped
	// form on its own.
	[[nodiscard]]
	std::string escaped_path() const;

	// This method is like 'set_path' but for 'fragment'/'raw_fragment'.
	void set_fragment(const std::string& f);

	// Returns the escaped fragment from 'fragment'.
	// Returns 'raw_fragment' when it is a valid escaping of 'fragment'.
	// Otherwise ignores 'raw_fragment' and computes an escaped
	// form on its own.
	[[nodiscard]]
	std::string escaped_fragment() const;

	// Reassembles the URL into a valid URL string.
	[[nodiscard]]
	std::string str() const;

	// Behaves like 'str()' but replaces any password with "xxxxx".
	// Only the password is redacted.
	[[nodiscard]]
	std::string redacted() const;

	// Resolves a URI reference to an absolute URI from
	// an absolute base URI, per RFC 3986 Section 5.2. The URI reference
	// may be relative or absolute. 'resolve_reference' always returns a new
	// URL instance, even if the returned URL is identical to either the
	// base or reference. If ref is an absolute URL, then 'resolve_reference'
	// ignores base and returns a copy of 'ref'.
	[[nodiscard]]
	URL resolve_reference(const URL& ref) const;

	// Returns the encoded path?query or opaque?query
	// string that would be used in an HTTP request.
	[[nodiscard]]
	std::string request_uri() const;

	// Parses 'raw_query' and returns the corresponding values.
	[[nodiscard]]
	inline Query query() const
	{
		return parse_query(this->raw_query);
	}
};

// TESTME: parse_url
// Parses 'raw_url' into a Url structure.
//
// The 'raw_url' may be relative (a path, without a host) or absolute
// (starting with a scheme). Trying to parse a hostname and path
// without a scheme is invalid but may not necessarily return an
// error, due to parsing ambiguities.
URL parse_url(const std::string& raw_url);



__HTTP_END__


__HTTP_INTERNAL_BEGIN__

// TESTME: parse_authority
extern std::pair<URL::UserInfo, std::string> parse_authority(const std::string& authority);

// TESTME: parse
extern URL parse_url(const std::string& raw_url, bool via_request);

__HTTP_INTERNAL_END__
