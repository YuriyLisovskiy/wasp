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

#include "url_parser.h"


__INTERNAL_BEGIN__

UrlParser::UrlParser() : _isParsed(false)
{
}

void UrlParser::checkIfParsed() const
{
	if (!this->_isParsed)
	{
		throw ParseError("url is not parsed", _ERROR_DETAILS_);
	}
}

std::string UrlParser::scheme() const
{
	this->checkIfParsed();
	return this->_url.scheme;
}

std::string UrlParser::username() const
{
	this->checkIfParsed();
	return this->_url.username;
}

std::string UrlParser::password() const
{
	this->checkIfParsed();
	return this->_url.password;
}

std::string UrlParser::hostname() const
{
	this->checkIfParsed();
	return this->_url.hostname;
}

std::string UrlParser::port() const
{
	this->checkIfParsed();
	return this->_url.port;
}

std::string UrlParser::path() const
{
	this->checkIfParsed();
	return this->_url.path;
}

std::string UrlParser::query() const
{
	this->checkIfParsed();
	return this->_url.query;
}

std::string UrlParser::fragment() const
{
	this->checkIfParsed();
	return this->_url.fragment;
}

uint16_t UrlParser::httpPort() const
{
	this->checkIfParsed();

	const uint16_t httpPort = 80;
	const uint16_t httpsPort = 443;

	if (this->_url.port.empty())
	{
		if (this->scheme() == "https")
		{
			return httpsPort;
		}
		else
		{
			return httpPort;
		}
	}
	else
	{
		return this->_url.integerPort;
	}
}

bool UrlParser::isUnreserved(char ch) const
{
	return std::isalnum(ch) || ch == '-' || ch == '.' || ch == '_' || ch == '~';
}

void UrlParser::parse(const std::string& str)
{
	std::string usernameOrHostname;
	std::string portOrPassword;

	this->_url = Url();
	this->_url.path = "/";
	this->_url.integerPort = 0;

	UrlParser::State state = UrlParser::State::Scheme;

	for (char ch : str)
	{
		switch (state)
		{
			case UrlParser::State::Scheme:
				if (std::isalnum(ch) || ch == '+' || ch == '-' || ch == '.')
				{
					this->_url.scheme += ch;
				}
				else if (ch == ':')
				{
					state = UrlParser::State::FirstSlashAfterScheme;
				}
				else
				{
					throw ParseError("unable to parse url scheme", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::FirstSlashAfterScheme:
				if (ch == '/')
				{
					state = UrlParser::State::SecondSlashAfterScheme;
				}
				else if (std::isalnum(ch))
				{
					usernameOrHostname = ch;
					state = UrlParser::State::UsernameOrHostname;
				}
				else
				{
					throw ParseError("unable to parse the first slash after scheme", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::SecondSlashAfterScheme:
				if (ch == '/')
				{
					state = UrlParser::State::UsernameOrHostname;
				}
				else
				{
					throw ParseError("unable to parse the second slash after scheme", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::UsernameOrHostname:
				if (this->isUnreserved(ch) || ch == '%')
				{
					usernameOrHostname += ch;
				}
				else if (ch == ':')
				{
					state = UrlParser::State::PortOrPassword;
				}
				else if (ch == '@')
				{
					state = UrlParser::State::Hostname;
					std::swap(this->_url.username, usernameOrHostname);
				}
				else if (ch == '/')
				{
					state = UrlParser::State::Path;
					std::swap(this->_url.hostname, usernameOrHostname);
				}
				else
				{
					throw ParseError("unable to parse username or hostname", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::Password:
				if (isalnum(ch) || ch == '%')
				{
					this->_url.password += ch;
				}
				else if (ch == '@')
				{
					state = UrlParser::State::Hostname;
				}
				else
				{
					throw ParseError("unable to parse password", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::Hostname:
				if (ch == '[' && this->_url.hostname.empty())
				{
					state = UrlParser::State::IPV6Hostname;
				}
				else if (isUnreserved(ch) || ch == '%')
				{
					this->_url.hostname += ch;
				}
				else if (ch == ':')
				{
					state = UrlParser::State::Port;
				}
				else if (ch == '/')
				{
					state = UrlParser::State::Path;
				}
				else
				{
					throw ParseError("unable to parse hostname", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::IPV6Hostname:
				std::abort(); // TODO
			case UrlParser::State::PortOrPassword:
				if (std::isdigit(ch))
				{
					portOrPassword += ch;
				}
				else if (ch == '/')
				{
					std::swap(this->_url.hostname, usernameOrHostname);
					std::swap(this->_url.port, portOrPassword);
					this->_url.integerPort = std::strtol(this->_url.port.c_str(), nullptr, 0);
					state = UrlParser::State::Path;
				}
				else if (std::isalnum(ch) || ch == '%')
				{
					std::swap(this->_url.username, usernameOrHostname);
					std::swap(this->_url.password, portOrPassword);
					this->_url.password += ch;
					state = UrlParser::State::Password;
				}
				else
				{
					throw ParseError("unable to parse port or password", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::Port:
				if (std::isdigit(ch))
				{
					portOrPassword += ch;
				}
				else if (ch == '/')
				{
					std::swap(this->_url.port, portOrPassword);
					this->_url.integerPort = strtol(this->_url.port.c_str(), nullptr, 0);
					state = Path;
				}
				else
				{
					throw ParseError("unable to parse port", _ERROR_DETAILS_);
				}
				break;
			case UrlParser::State::Path:
				if (ch == '#')
				{
					state = UrlParser::State::Fragment;
				}
				else if (ch == '?')
				{
					state = UrlParser::State::Query;
				}
				else
				{
					this->_url.path += ch;
				}
				break;
			case UrlParser::State::Query:
				if (ch == '#')
				{
					state = UrlParser::State::Fragment;
				}
				else if (ch == '?')
				{
					state = UrlParser::State::Query;
				}
				else
				{
					this->_url.query += ch;
				}
				break;
			case UrlParser::State::Fragment:
				this->_url.fragment += ch;
				break;
		}
	}

	assert(portOrPassword.empty());

	if (!usernameOrHostname.empty())
	{
		this->_url.hostname = usernameOrHostname;
	}

	this->_isParsed = true;
}

__INTERNAL_END__
