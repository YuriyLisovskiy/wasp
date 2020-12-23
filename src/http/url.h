/**
 * http/url.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Represents url as Url object.
 * 	Parses url from string using url parser.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

class Url final
{
private:
	std::string _str_url;
	std::string _scheme;
	std::string _username;
	std::string _password;
	std::string _hostname;
	std::string _port;
	std::string _path;
	std::string _query;
	std::string _fragment;
	uint16_t _integer_port;

public:
	explicit Url(const std::string& url);
	explicit Url(const char* url);

	[[nodiscard]] std::string scheme() const;
	[[nodiscard]] std::string username() const;
	[[nodiscard]] std::string password() const;
	[[nodiscard]] std::string hostname() const;
	[[nodiscard]] uint16_t port() const;
	[[nodiscard]] std::string path() const;
	[[nodiscard]] std::string query() const;
	[[nodiscard]] std::string fragment() const;
	[[nodiscard]] std::string str() const;
};

__HTTP_END__
