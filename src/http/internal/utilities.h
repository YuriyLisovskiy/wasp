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

// Module definitions.
#include "./_def_.h"


__HTTP_INTERNAL_UTIL_BEGIN__

inline const std::map<char, bool> is_token_table = {
	{'!', true},
	{'#', true},
	{'$', true},
	{'%', true},
	{'&', true},
	{'\'',true},
	{'*', true},
	{'+', true},
	{'-', true},
	{'.', true},
	{'0', true},
	{'1', true},
	{'2', true},
	{'3', true},
	{'4', true},
	{'5', true},
	{'6', true},
	{'7', true},
	{'8', true},
	{'9', true},
	{'A', true},
	{'B', true},
	{'C', true},
	{'D', true},
	{'E', true},
	{'F', true},
	{'G', true},
	{'H', true},
	{'I', true},
	{'J', true},
	{'K', true},
	{'L', true},
	{'M', true},
	{'N', true},
	{'O', true},
	{'P', true},
	{'Q', true},
	{'R', true},
	{'S', true},
	{'T', true},
	{'U', true},
	{'W', true},
	{'V', true},
	{'X', true},
	{'Y', true},
	{'Z', true},
	{'^', true},
	{'_', true},
	{'`', true},
	{'a', true},
	{'b', true},
	{'c', true},
	{'d', true},
	{'e', true},
	{'f', true},
	{'g', true},
	{'h', true},
	{'i', true},
	{'j', true},
	{'k', true},
	{'l', true},
	{'m', true},
	{'n', true},
	{'o', true},
	{'p', true},
	{'q', true},
	{'r', true},
	{'s', true},
	{'t', true},
	{'u', true},
	{'v', true},
	{'w', true},
	{'x', true},
	{'y', true},
	{'z', true},
	{'|', true},
	{'~', true}
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
	return b < is_token_table.size() && is_token_table.find(b) != is_token_table.end() && is_token_table.at(b);
}

// TESTME: is_not_token
inline bool is_not_token(char b)
{
	return !is_token_byte(b);
}

__HTTP_INTERNAL_UTIL_END__
