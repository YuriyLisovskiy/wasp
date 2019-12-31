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
 * An implementation of url.h.
 */

#include "./url.h"


__HTTP_BEGIN__

Url::Url(const std::string& url)
{
	core::internal::url_parser parser;
	parser.parse(url);
	if (!parser.is_parsed)
	{
		throw core::ParseError(parser.last_err, parser.err_line, parser.err_func, parser.err_file);
	}

	this->_str_url = url;
	this->_scheme = parser.scheme;
	this->_username = parser.username;
	this->_password = parser.password;
	this->_hostname = parser.hostname;
	this->_port = parser.port;
	this->_path = parser.path;
	this->_query = parser.query;
	this->_fragment = parser.fragment;
	this->_integer_port = parser.integer_port;
}

Url::Url(const char* url) : Url(std::string(url))
{
}

std::string Url::scheme() const
{
	return this->_scheme;
}

std::string Url::username() const
{
	return this->_username;
}

std::string Url::password() const
{
	return this->_password;
}

std::string Url::hostname() const
{
	return this->_hostname;
}

std::string Url::path() const
{
	return this->_path;
}

std::string Url::query() const
{
	return this->_query;
}

std::string Url::fragment() const
{
	return this->_fragment;
}

uint16_t Url::port() const
{
	uint16_t port;
	const uint16_t http_port = 80, https_port = 443;
	if (this->_port.empty())
	{
		if (this->scheme() == "https")
		{
			port = https_port;
		}
		else
		{
			port = http_port;
		}
	}
	else
	{
		port = this->_integer_port;
	}

	return port;
}

std::string Url::str() const
{
	return this->_str_url;
}

__HTTP_END__
