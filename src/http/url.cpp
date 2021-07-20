/**
 * http/url.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./url.h"

// Base libraries.
#include <xalwart.base/exceptions.h>

// Framework libraries.
#include "../core/parsers/url_parser.h"


__HTTP_BEGIN__

Url::Url(const std::string& url)
{
	parsers::url_parser parser;
	parser.parse(url);
	if (!parser.is_parsed)
	{
		throw ParseError(parser.last_err, parser.err_line, parser.err_func, parser.err_file);
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
