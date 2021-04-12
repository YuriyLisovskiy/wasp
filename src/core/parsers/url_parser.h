/**
 * core/parsers/url_parser.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * URL parser.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "../_def_.h"


__PARSERS_BEGIN__

// TESTME: url_parser
struct url_parser final
{
	enum state
	{
		s_scheme,
		s_first_slash,
		s_second_slash,
		s_username_or_hostname,
		s_password,
		s_hostname,
		s_ipv6_hostname,
		s_ipv6_hostname_end,
		s_port_or_password,
		s_port,
		s_path,
		s_query,
		s_fragment
	};

	bool is_parsed;
	const char* last_err;
	long int err_line;
	const char* err_file;
	const char* err_func;

	std::string scheme;
	std::string username;
	std::string password;
	std::string hostname;
	std::string port;
	std::string path;
	std::string query;
	std::string fragment;
	uint16_t integer_port;

	// Default constructor.
	inline url_parser() : is_parsed(false), last_err(""), err_line(-1),
		err_func(""), err_file(""), integer_port(0)
	{
	}

	// Parses URL from given string. Extracts scheme, username (optional),
	// password (optional), hostname, port (optional), path (optional),
	// query (optional) and fragment (optional) from given URL.
	//
	// `str`: input URL.
	void parse(const std::string& str);

	// Returns `true` if char `ch` is alphanumeric or equals to one of
	// this symbols: - . _ ~, `false` otherwise.
	//
	// `ch`: symbol to check.
	inline static bool is_unreserved(char ch)
	{
		return std::isalnum(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~';
	}

	// Returns `true` if char `ch` is digit or equals to one of
	// this symbols: a b c d e f :, `false` otherwise.
	//
	// `ch`: symbol to check.
	inline static bool is_ipv6_symbol(char ch)
	{
		return std::isdigit(ch) || ch == ':' || ch == 'a' || ch == 'b' ||
			ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f';
	}

	// Just sets an error.
	inline void set_err(const char* err, int line, const char* func, const char* file)
	{
		this->last_err = err, this->err_line = line, this->err_func = func, this->err_file = file;
	}
};

__PARSERS_END__
