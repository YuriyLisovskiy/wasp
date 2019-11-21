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
 * An implementation of url_parser.h
 *
 * @author Yuriy Lisovskiy
 */

#include "url_parser.h"


__INTERNAL_BEGIN__

url_parser::url_parser() : is_parsed(false)
{
	this->integer_port = 0;
}

void url_parser::parse(const std::string& str)
{
	std::string username_or_hostname, port_or_password;
	this->path = "/";
	url_parser::state st = url_parser::state::s_scheme;
	for (char ch : str)
	{
		switch (st)
		{
			case url_parser::state::s_scheme:
				if (std::isalnum(ch) || ch == '+' || ch == '-' || ch == '.')
				{
					this->scheme += ch;
				}
				else if (ch == ':')
				{
					st = url_parser::state::s_first_slash;
				}
				else
				{
					throw ParseError("unable to parse url scheme", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_first_slash:
				if (ch == '/')
				{
					st = url_parser::state::s_second_slash;
				}
				else if (std::isalnum(ch))
				{
					username_or_hostname = ch;
					st = url_parser::state::s_username_or_hostname;
				}
				else
				{
					throw ParseError("unable to parse the first slash after scheme", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_second_slash:
				if (ch == '/')
				{
					st = url_parser::state::s_username_or_hostname;
				}
				else
				{
					throw ParseError("unable to parse the second slash after scheme", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_username_or_hostname:
				if (url_parser::is_unreserved(ch) || ch == '%')
				{
					username_or_hostname += ch;
				}
				else if (ch == ':')
				{
					st = url_parser::state::s_port_or_password;
				}
				else if (ch == '@')
				{
					st = url_parser::state::s_hostname;
					std::swap(this->username, username_or_hostname);
				}
				else if (ch == '/')
				{
					st = url_parser::state::s_path;
					std::swap(this->hostname, username_or_hostname);
				}
				else
				{
					throw ParseError("unable to parse username or hostname", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_password:
				if (isalnum(ch) || ch == '%')
				{
					this->password += ch;
				}
				else if (ch == '@')
				{
					st = url_parser::state::s_hostname;
				}
				else
				{
					throw ParseError("unable to parse password", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_hostname:
				if (ch == '[' && this->hostname.empty())
				{
					st = url_parser::state::s_ipv6_hostname;
				}
				else if (url_parser::is_unreserved(ch) || ch == '%')
				{
					this->hostname += ch;
				}
				else if (ch == ':')
				{
					st = url_parser::state::s_port;
				}
				else if (ch == '/')
				{
					st = url_parser::state::s_path;
				}
				else
				{
					throw ParseError("unable to parse hostname", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_ipv6_hostname:
				std::abort(); // TODO
			case url_parser::state::s_port_or_password:
				if (std::isdigit(ch))
				{
					port_or_password += ch;
				}
				else if (ch == '/')
				{
					std::swap(this->hostname, username_or_hostname);
					std::swap(this->port, port_or_password);
					this->integer_port = std::strtol(this->port.c_str(), nullptr, 0);
					st = url_parser::state::s_path;
				}
				else if (std::isalnum(ch) || ch == '%')
				{
					std::swap(this->username, username_or_hostname);
					std::swap(this->password, port_or_password);
					this->password += ch;
					st = url_parser::state::s_password;
				}
				else
				{
					throw ParseError("unable to parse port or password", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_port:
				if (std::isdigit(ch))
				{
					port_or_password += ch;
				}
				else if (ch == '/')
				{
					std::swap(this->port, port_or_password);
					this->integer_port = strtol(this->port.c_str(), nullptr, 0);
					st = s_path;
				}
				else
				{
					throw ParseError("unable to parse port", _ERROR_DETAILS_);
				}
				break;
			case url_parser::state::s_path:
				if (ch == '#')
				{
					st = url_parser::state::s_fragment;
				}
				else if (ch == '?')
				{
					st = url_parser::state::s_query;
				}
				else
				{
					this->path += ch;
				}
				break;
			case url_parser::state::s_query:
				if (ch == '#')
				{
					st = url_parser::state::s_fragment;
				}
				else if (ch == '?')
				{
					st = url_parser::state::s_query;
				}
				else
				{
					this->query += ch;
				}
				break;
			case url_parser::state::s_fragment:
				this->fragment += ch;
				break;
		}
	}

	assert(port_or_password.empty());
	if (!username_or_hostname.empty())
	{
		this->hostname = username_or_hostname;
	}

	this->is_parsed = true;
}

void url_parser::parse(const char* str)
{
	this->parse(std::string(str));
}

bool url_parser::is_unreserved(char ch)
{
	return std::isalnum(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~';
}

__INTERNAL_END__
