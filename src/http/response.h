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
 * HTTP/1.1 responses definition.
 * TODO: write docs
 */

#ifndef WASP_HTTP_RESPONSE_H
#define WASP_HTTP_RESPONSE_H

#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <iterator>

#include "../globals.h"
#include "request.h"
#include "cookie.h"
#include "../core/exceptions.h"
#include "status.h"
#include "../utils/str.h"

#include "../utils/datetime/datetime.h"


__WASP_BEGIN__

// An HTTP response base class with dictionary-accessed headers.
//
// This class doesn't handle content. It should not be used directly.
// Use the HttpResponse and StreamingHttpResponse subclasses instead.
class HttpResponseBase
{
protected:
	Dict<std::string, std::string> _headers;
	Dict<std::string, Cookie> _cookies;
	std::string _body;
	bool _closed;
	unsigned short int _status;
	std::string _charset;
	std::string _reasonPhrase;
	bool _streaming;

	std::string serializeHeaders();

public:
	explicit HttpResponseBase(
		unsigned short int status = 0,
		std::string contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	virtual ~HttpResponseBase() = default;
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
	void deleteCookie(const std::string& name, const std::string& path, const std::string& domain);

	std::string getReasonPhrase();
	void setReasonPhrase(std::string value);

	// These methods partially implement the file-like object interface.
	virtual void write(const std::string& content);
	virtual unsigned long int tell();

	// These methods partially implement a stream-like object interface.
	virtual bool writable();
	virtual void writeLines(const std::vector<std::string>& lines);

	virtual std::string serialize() = 0;
};

// An HTTP response class with a string as content.
//
// This content that can be read, appended to, or replaced.
class HttpResponse : public HttpResponseBase
{
private:
	std::string _content;

public:
	explicit HttpResponse(
		const std::string& content,
		unsigned short int status = 200,
		const std::string& contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	void write(const std::string& content) override;
	unsigned long int tell() override;
	bool writable() override;
	void writeLines(const std::vector<std::string>& lines) override;
	std::string serialize() override;
};

__WASP_END__


#endif // WASP_HTTP_RESPONSE_H
