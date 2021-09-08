/**
 * http/cookie/parser.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * HTTP cookie parser.
 *
 * Implementation of parser is based on Golang 1.15.8 linux/amd64:
 * https://github.com/golang/go/blob/master/src/net/http/cookie.go
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <map>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "./cookie.h"
#include "../utility.h"


__HTTP_BEGIN__

// TESTME: is_ascii_space
inline bool is_ascii_space(char b)
{
	return b == ' ' || b == '\t' || b == '\n' || b == '\r';
}

// TESTME: valid_cookie_value_byte
inline bool valid_cookie_value_byte(char b)
{
	return 0x20 <= b && b < 0x7f && b != '"' && b != ';' && b != '\\';
}

// TESTME: parse_cookie_value
extern std::pair<std::string, bool> parse_cookie_value(std::string raw_val, bool allow_double_quote);

//extern bool is_token_table();

// TESTME: is_cookie_name_valid
inline bool is_cookie_name_valid(const std::string& raw)
{
	return !raw.empty() && std::find_if(raw.begin(), raw.end(), is_not_token) == std::end(raw);
}

// TESTME: parse_cookies
// Parses all "Cookie" values from the header and
// returns the successfully parsed Cookies.
//
// if filter isn't empty, only cookies of that name are returned
extern std::vector<Cookie> parse_cookies(std::string raw_cookie, const std::string& filter);

__HTTP_END__
