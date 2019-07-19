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
 * http response definition.
 * TODO: write docs
 */

#ifndef WASP_HTTP_RESPONSE_H
#define WASP_HTTP_RESPONSE_H

#include <string>
#include <map>
#include <vector>

#include "../globals.h"
#include "request.h"
#include "cookie.h"
#include "../core/exceptions.h"
#include "status.h"

#include "../utils/datetime/datetime.h"


__WASP_BEGIN__

class HttpResponseBase
{
private:
	QueryDict<std::string, std::string> _headers;
	QueryDict<std::string, Cookie> _cookies;
	std::string _body;
	bool _closed;
	unsigned short int _status;
	std::string _charset;
	std::string _reasonPhrase;

public:
	explicit HttpResponseBase(
		unsigned short int status = 0,
		std::string contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	void setHeader(const std::string& key, const std::string& value);
	void removeHeader(const std::string& key);
	bool hasHeader(const std::string& key);

	void setCookie(
		const std::string& name,
		const std::string& value,
		const std::string& expires = "",
		const std::string& domain = "",
		const std::string& path = "/",
		bool isSecure = false,
		bool isHttpOnly = false
	);
	void setSignedCookie(
		const std::string& name,
	    const std::string& value,
		const std::string& salt = "",
	    const std::string& expires = "",
	    const std::string& domain = "",
	    const std::string& path = "/",
	    bool isSecure = false,
	    bool isHttpOnly = false
	);
	void deleteCookie(const std::string& key);

	std::string getReasonPhrase();
	void setReasonPhrase(std::string value);

	std::string toString();
};

__WASP_END__


#endif // WASP_HTTP_RESPONSE_H
