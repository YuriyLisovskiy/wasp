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
 * url.h
 * Purpose: represents url as Url object.
 * 			Parses url from string using url parser.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"
#include "../core/parsers/url_parser.h"


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
