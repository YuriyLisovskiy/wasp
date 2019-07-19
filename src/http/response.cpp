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
 * http response implementation.
 * TODO: write docs
 */

#include "response.h"


__WASP_BEGIN__

HttpResponseBase::HttpResponseBase(
	unsigned short int status,
    std::string contentType,
	const std::string& reason,
    const std::string& charset
)
{
	this->_headers = QueryDict<std::string, std::string>(true);
	this->_closed = false;

	if (status != 0)
	{
		if (status < 100 || status > 599)
		{
			throw ValueError("HTTP status code must be an integer from 100 to 599.", _ERROR_DETAILS_);
		}
		this->_status = status;
	}
	else
	{
		this->_status = 200;
	}

	this->_charset = charset;

	if (contentType.empty())
	{
		contentType = "text/html; charset=" + this->_charset;
	}
	this->_headers.set("Content-Type", contentType);
}

void HttpResponseBase::setHeader(const std::string& key, const std::string& value)
{
	this->_headers.set(key, value);
}

void HttpResponseBase::removeHeader(const std::string& key)
{
	this->_headers.remove(key);
}

bool HttpResponseBase::hasHeader(const std::string& key)
{
	return this->_headers.contains(key);
}

void HttpResponseBase::setCookie(
	const std::string& name,
	const std::string& value,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool isSecure,
	bool isHttpOnly
)
{
	this->_cookies.set(name, Cookie(name, value, expires, domain, path, isSecure, isHttpOnly));
}

void HttpResponseBase::setSignedCookie(
	const std::string& name,
	const std::string& value,
	const std::string& salt,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool isSecure,
	bool isHttpOnly
)
{
	// TODO:
}

void HttpResponseBase::deleteCookie(const std::string& name)
{
	// TODO:
}

std::string HttpResponseBase::getReasonPhrase()
{
	if (!this->_reasonPhrase.empty())
	{
		return this->_reasonPhrase;
	}
	if (internal::HTTP_STATUS.contains(this->_status))
	{
		return internal::HTTP_STATUS.get(this->_status).first;
	}
	return "Unknown Status Code";
}

void HttpResponseBase::setReasonPhrase(std::string value)
{
	if (value.empty())
	{
		value = "Unknown Status Code";
	}
	this->_reasonPhrase = value;
}

// TODO: build response
std::string HttpResponseBase::toString()
{
	std::string body("<form action=\"/hello\" method=\"post\" enctype=\"multipart/form-data\">\n"
					 "\t<input type=\"file\" name=\"super_file\" />\n"
					 "\t<input type=\"password\" name=\"first_name\" />\n"
					 "\t<input type=\"submit\" value=\"send\" />\n"
					 "\t</form>\n"
	                 "\t<img src=\"../../../Screenshot from 2019-07-17 17-19-44.png\" alt=\"image\" />");

	this->_headers.set("Date", dt::now().strftime("%a, %d %b %Y %T %Z"));

	return std::string("HTTP/1.1 200 OK\n"
				//	"Content-Length: " + std::to_string(body.size()) + "\n"
					"Connection: close\n"
					"Content-Type: text/html\n"
					"Server: Apache/2.2.8 (Ubuntu) mod_ssl/2.2.8 OpenSSL/0.9.8g\n\n" +
				  body);
}

__WASP_END__
