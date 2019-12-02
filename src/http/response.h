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
#include <set>
#include <fstream>

#include "../globals.h"
#include "request.h"
#include "cookie.h"
#include "../core/exceptions.h"
#include "status.h"
#include "url.h"
#include "../utility/str.h"
#include "../utility/encoding.h"
#include "../utility/path.h"
#include "../utility/mime_types.h"

#include "../utility/datetime/datetime.h"


__WASP_BEGIN__

// An HTTP response base class with dictionary-accessed headers.
//
// This class doesn't handle content. It should not be used directly.
// Use the HttpResponse subclass instead.
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
	virtual void close();
	virtual void write(const std::string& content);
	virtual void flush();
	virtual unsigned long int tell();

	// These methods partially implement a stream-like object interface.
	virtual bool readable();
	virtual bool seekable();
	virtual bool writable();
	virtual void writeLines(const std::vector<std::string>& lines);

	virtual std::string serialize() = 0;
};


// An HTTP response class with a string as content.
class HttpResponse : public HttpResponseBase
{
protected:
	std::string _content;

public:
	explicit HttpResponse(
		const std::string& content,
		unsigned short int status = 200,
		const std::string& contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	virtual void setContent(const std::string& content);
	void write(const std::string& content) override;
	unsigned long int tell() override;
	bool writable() override;
	void writeLines(const std::vector<std::string>& lines) override;
	std::string serialize() override;
};


class StreamingHttpResponse : public HttpResponseBase
{
public:
	explicit StreamingHttpResponse(
		unsigned short int status = 0,
		const std::string& contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string serialize() final;
	virtual std::string getChunk() = 0;
};


class FileResponse final : public StreamingHttpResponse
{
private:
	static const size_t CHUNK_SIZE = 1024 * 1024;   // 1 mb per chunk

	bool _asAttachment;
	std::string _filePath;

	size_t _bytesRead;
	size_t _totalBytesRead;
	size_t _fileSize;
	std::ifstream _fileStream;

	// Identifies whether headers where read or not.
	bool _headersIsGot;

	void _setHeaders();
	std::string _getHeadersChunk();

public:
	explicit FileResponse(
		std::string  filePath,
		bool asAttachment = false,
		unsigned short int status = 0,
		const std::string& contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string getChunk() override;
	void close() override;
	unsigned long int tell() override;
};


class HttpResponseRedirectBase : public HttpResponse
{
protected:
	const std::set<std::string> _allowedHosts = {"http_tests", "https", "ftp"};

public:
	explicit HttpResponseRedirectBase(
		const std::string& redirectTo,
		const std::string& content,
		unsigned short int status,
		const std::string& contentType = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string url();
};


class HttpResponseRedirect : public HttpResponseRedirectBase
{
public:
	explicit HttpResponseRedirect(
		const std::string& redirectTo,
		const std::string& content = "",
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponsePermanentRedirect : public HttpResponseRedirectBase
{
public:
	explicit HttpResponsePermanentRedirect(
		const std::string& redirectTo,
		const std::string& content = "",
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotModified : public HttpResponse
{
public:
	explicit HttpResponseNotModified(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
	void setContent(const std::string& content) override;
};


class HttpResponseBadRequest : public HttpResponse
{
public:
	explicit HttpResponseBadRequest(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotFound : public HttpResponse
{
public:
	explicit HttpResponseNotFound(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseForbidden : public HttpResponse
{
public:
	explicit HttpResponseForbidden(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotAllowed : public HttpResponse
{
public:
	explicit HttpResponseNotAllowed(
		const std::string& content,
		const std::vector<std::string>& permittedMethods,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseGone : public HttpResponse
{
public:
	explicit HttpResponseGone(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseEntityTooLarge : public HttpResponse
{
public:
	explicit HttpResponseEntityTooLarge(
			const std::string& content,
			const std::string& contentType = "",
			const std::string& charset = "utf-8"
	);
};


class HttpResponseServerError : public HttpResponse
{
public:
	explicit HttpResponseServerError(
		const std::string& content,
		const std::string& contentType = "",
		const std::string& charset = "utf-8"
	);
};


// TODO: implement JsonResponse!

__WASP_END__


#endif // WASP_HTTP_RESPONSE_H
