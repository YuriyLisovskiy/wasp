/**
 * http/internal/utilities.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Http internal utilities.
 *
 * Implementation of parser is based on Golang 1.15.8 linux/amd64.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <algorithm>

// Module definitions.
#include "./_def_.h"


__HTTP_INTERNAL_UTIL_BEGIN__

inline const short token_table_size = 77;
inline const char token_table[token_table_size] = {
	'!', '#', '$', '%', '&', '\'', '*', '+', '-', '.',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'^', '_', '`',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'|', '~'
};

// TESTME: is_ascii_space
inline bool is_ascii_space(char b)
{
	return b == ' ' || b == '\t' || b == '\n' || b == '\r';
}

// TESTME: trim_ascii_space
// Trims leading and trailing ASCII space of 's'.
extern void trim_ascii_space(std::string& s);

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

__HTTP_INTERNAL_UTIL_END__
