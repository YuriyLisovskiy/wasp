/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * An implementation of core/parsers/url_parser.h
 */

#include "./url_parser.h"


__CORE_INTERNAL_BEGIN__

url_parser::url_parser()
	: is_parsed(false), last_err(""), err_line(-1), err_func(""), err_file(""), is_reset(true)
{
	this->integer_port = 0;
}

void url_parser::parse(const std::string& str)
{
	this->reset();
	this->is_reset = false;
	std::string username_or_hostname, port_or_password;
	this->path = "/";
	url_parser::state st = url_parser::state::s_scheme;
	str::ltrim(str, "/");
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
				else if (ch == '/' && this->scheme.empty())
				{
					st = url_parser::state::s_path;
				}
				else
				{
					this->set_err("unable to parse url scheme", _ERROR_DETAILS_);
					return;
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
					this->set_err("unable to parse the first slash after scheme", _ERROR_DETAILS_);
					return;
				}
				break;
			case url_parser::state::s_second_slash:
				if (ch == '/')
				{
					st = url_parser::state::s_username_or_hostname;
				}
				else
				{
					this->set_err("unable to parse the second slash after scheme", _ERROR_DETAILS_);
					return;
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
				else if (ch == '[' && username_or_hostname.empty())
				{
					st = url_parser::state::s_ipv6_hostname;
				}
				else
				{
					this->set_err("unable to parse username or hostname", _ERROR_DETAILS_);
					return;
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
					this->set_err("unable to parse password", _ERROR_DETAILS_);
					return;
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
					this->set_err("unable to parse hostname", _ERROR_DETAILS_);
					return;
				}
				break;
			case url_parser::state::s_ipv6_hostname:
				if (ch == ']')
				{
					st = url_parser::state::s_ipv6_hostname_end;
				}
				else if (url_parser::is_ipv6_symbol(ch))
				{
					this->hostname += ch;
				}
				else
				{
					this->set_err("unable to parse IPv6 hostname", _ERROR_DETAILS_);
					return;
				}
				break;
			case url_parser::state::s_ipv6_hostname_end:
				if (ch == ':')
				{
					st = url_parser::state::s_port;
				}
				else if (ch == '/')
				{
					st = url_parser::state::s_path;
				}
				else
				{
					this->set_err("unable to parse IPv6 hostname", _ERROR_DETAILS_);
					return;
				}
				break;
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
					this->set_err("unable to parse port or password", _ERROR_DETAILS_);
					return;
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
					this->set_err("unable to parse port", _ERROR_DETAILS_);
					return;
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

	if (!port_or_password.empty())
	{
		this->set_err("port and password are not empty", _ERROR_DETAILS_);
		return;
	}

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

void url_parser::reset()
{
	if (this->is_reset)
	{
		return;
	}

	this->is_parsed = false;
	this->last_err = "";
	this->err_line = -1;
	this->err_file = "";
	this->err_func = "";

	this->scheme.clear();
	this->username.clear();
	this->password.clear();
	this->hostname.clear();
	this->port.clear();
	this->path.clear();
	this->query.clear();
	this->fragment.clear();
	this->integer_port = 0;

	this->is_reset = true;
}

bool url_parser::is_unreserved(char ch)
{
	return std::isalnum(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~';
}

bool url_parser::is_ipv6_symbol(char ch)
{
	return std::isdigit(ch) || ch == ':' || ch == 'a' || ch == 'b' ||
		ch == 'c' || ch == 'd' || ch == 'e' || ch == 'f';
}

void url_parser::set_err(const char* err, int line, const char* func, const char* file)
{
	this->last_err = err;
	this->err_line = line,
	this->err_func = func;
	this->err_file = file;
}

__CORE_INTERNAL_END__
