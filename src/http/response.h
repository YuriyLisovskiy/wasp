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
 * response.h
 * Purpose: contains base http response and most frequently
 * 			used http response.
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>
#include <vector>
#include <cstring>
#include <iterator>
#include <set>
#include <fstream>
#include <utility>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./request.h"
#include "./cookie.h"
#include "../core/exceptions.h"
#include "./status.h"
#include "./url.h"
#include "../core/string/str.h"
#include "../core/encoding.h"
#include "../core/path.h"
#include "../core/mime_types.h"
#include "../core/datetime/datetime.h"


__HTTP_BEGIN__

/// An HTTP response base class with dictionary-accessed headers.
///
/// This class doesn't handle content. It should not be used directly.
/// Use the HttpResponse subclass instead.
class HttpResponseBase
{
protected:
	collections::Dict<std::string, std::string> _headers;
	collections::Dict<std::string, Cookie> _cookies;
	std::string _body;
	bool _closed;
	unsigned short int _status;
	std::string _charset;
	std::string _reason_phrase;
	bool _streaming;

	std::string serialize_headers();

public:
	explicit HttpResponseBase(
		unsigned short int status = 0,
		std::string content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	virtual ~HttpResponseBase() = default;
	void set_header(const std::string& key, const std::string& value);
	void remove_header(const std::string& key);
	bool has_header(const std::string& key);

	void set_cookie(
		const std::string& name,
		const std::string& value,
		const std::string& expires = "",
		const std::string& domain = "",
		const std::string& path = "/",
		bool is_secure = false,
		bool is_http_only = false
	);
	void set_signed_cookie(
		const std::string& name,
	    const std::string& value,
		const std::string& salt = "",
	    const std::string& expires = "",
	    const std::string& domain = "",
	    const std::string& path = "/",
	    bool is_secure = false,
	    bool is_http_only = false
	);
	void delete_cookie(const std::string& name, const std::string& path, const std::string& domain);

	std::string get_reason_phrase();
	void set_reason_phrase(std::string value);

	unsigned short int status();
	std::string content_type();
	virtual size_t content_length();
	std::string charset();
	bool is_streaming();

	// These methods partially implement the file-like object interface.
	virtual void close();
	virtual void write(const std::string& content);
	virtual void flush();
	virtual unsigned long int tell();

	/// These methods partially implement a stream-like object interface.
	virtual bool readable();
	virtual bool seekable();
	virtual bool writable();
	virtual void write_lines(const std::vector<std::string>& lines);

	virtual std::string serialize() = 0;
};


/// An HTTP response class with a string as content.
class HttpResponse : public HttpResponseBase
{
protected:
	std::string _content;

public:
	explicit HttpResponse(
		const std::string& content,
		unsigned short int status = 200,
		const std::string& content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	size_t content_length() override;
	virtual void set_content(const std::string& content);
	void write(const std::string& content) override;
	unsigned long int tell() override;
	bool writable() override;
	void write_lines(const std::vector<std::string>& lines) override;
	std::string serialize() override;
};


class StreamingHttpResponse : public HttpResponseBase
{
public:
	explicit StreamingHttpResponse(
		unsigned short int status = 0,
		const std::string& content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string serialize() final;
	virtual std::string get_chunk() = 0;
};


class FileResponse final : public StreamingHttpResponse
{
private:
	static const size_t CHUNK_SIZE = 1024 * 1024;   // 1 mb per chunk

	bool _as_attachment;
	std::string _file_path;

	size_t _bytes_read;
	size_t _total_bytes_read;
	size_t _file_size;
	std::ifstream _file_stream;

	// Identifies whether headers where read or not.
	bool _headers_is_got;

	void _set_headers();
	std::string _get_headers_chunk();

public:
	explicit FileResponse(
		std::string file_path,
		bool as_attachment = false,
		unsigned short int status = 0,
		const std::string& content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string get_chunk() override;
	void close() override;
	unsigned long int tell() override;
};


class HttpResponseRedirectBase : public HttpResponse
{
protected:
	const std::set<std::string> _allowed_schemes = {"http", "https", "ftp"};

public:
	explicit HttpResponseRedirectBase(
		const std::string& redirect_to,
		unsigned short int status,
		const std::string& content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	std::string url();
};


class HttpResponseRedirect : public HttpResponseRedirectBase
{
public:
	explicit HttpResponseRedirect(
		const std::string& redirect_to,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponsePermanentRedirect : public HttpResponseRedirectBase
{
public:
	explicit HttpResponsePermanentRedirect(
		const std::string& redirect_to,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotModified : public HttpResponse
{
public:
	explicit HttpResponseNotModified(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
	void set_content(const std::string& content) override;
};


class HttpResponseBadRequest : public HttpResponse
{
public:
	explicit HttpResponseBadRequest(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotFound : public HttpResponse
{
public:
	explicit HttpResponseNotFound(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseForbidden : public HttpResponse
{
public:
	explicit HttpResponseForbidden(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseNotAllowed : public HttpResponse
{
public:
	explicit HttpResponseNotAllowed(
		const std::string& content,
		const std::vector<std::string>& permitted_methods,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseGone : public HttpResponse
{
public:
	explicit HttpResponseGone(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseEntityTooLarge : public HttpResponse
{
public:
	explicit HttpResponseEntityTooLarge(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};


class HttpResponseServerError : public HttpResponse
{
public:
	explicit HttpResponseServerError(
		const std::string& content,
		const std::string& content_type = "",
		const std::string& charset = "utf-8"
	);
};

__HTTP_END__
