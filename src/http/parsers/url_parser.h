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

/*
 * url_parser definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_PARSERS_URL_PARSER_H
#define WASP_HTTP_PARSERS_URL_PARSER_H

#include <string>
#include <cstdlib>
#include <cstdint>
#include <cassert>
#include <cstdlib>

#include "../../globals.h"
#include "../../core/exceptions.h"


__INTERNAL_BEGIN__

class UrlParser
{
private:
	enum ParserState
	{
		Scheme,
		FirstSlashAfterScheme,
		SecondSlashAfterScheme,
		UsernameOrHostname,
		Password,
		Hostname,
		IPV6Hostname,
		PortOrPassword,
		Port,
		Path,
		Query,
		Fragment
	};

protected:
	bool isUnreserved(char ch) const;

	bool _isParsed;

	struct Url
	{
		Url() : integerPort(0)
		{}

		std::string scheme;
		std::string username;
		std::string password;
		std::string hostname;
		std::string port;
		std::string path;
		std::string query;
		std::string fragment;
		uint16_t integerPort;
	} _url;

public:
	UrlParser();

	void parse(const std::string& str);

	void checkIfParsed() const;

	std::string scheme() const;
	std::string username() const;
	std::string password() const;
	std::string hostname() const;
	std::string port() const;
	std::string path() const;
	std::string query() const;
	std::string fragment() const;
	uint16_t httpPort() const;
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_URL_PARSER_H
