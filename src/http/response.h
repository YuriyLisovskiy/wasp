/**
 * http/response.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Contains base http response and most frequently
 * 	used http response.
 */

#pragma once

// C++ libraries.
#include <set>
#include <fstream>

// Base libraries.
#include <xalwart.base/collections/dictionary.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./interfaces.h"


__HTTP_BEGIN__

/// An HTTP response base class with dictionary-accessed headers.
///
/// This class doesn't handle content. It should not be used directly.
/// Use the HttpResponse subclass instead.
class HttpResponseBase : public IHttpResponse
{
protected:
	collections::Dictionary<std::string, std::string> _headers;
	collections::Dictionary<std::string, Cookie> _cookies;
	std::string _body;
	bool _closed;
	unsigned short int _status;
	std::string _charset;
	std::string _reason_phrase;
	bool _streaming;
	Error _err;

	std::string serialize_headers();

public:
	explicit HttpResponseBase(
		unsigned short int status = 0,
		std::string content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	~HttpResponseBase() override = default;
	std::string get_header(
		const std::string& key, const std::string& default_value
	) final;
	void set_header(const std::string& key, const std::string& value) final;
	void remove_header(const std::string& key) final;
	bool has_header(const std::string& key) final;
	void set_content(const std::string& content) override;
	std::string get_content() override;

	void set_cookie(
		const std::string& name,
		const std::string& value,
		long max_age = -1,
		const std::string& expires = "",
		const std::string& domain = "",
		const std::string& path = "/",
		bool is_secure = false,
		bool is_http_only = false,
		const std::string& same_site = ""
	) final;
	void set_signed_cookie(
		const std::string& secret_key,
		const std::string& name,
	    const std::string& value,
		const std::string& salt = "",
		long max_age = -1,
	    const std::string& expires = "",
	    const std::string& domain = "",
	    const std::string& path = "/",
	    bool is_secure = false,
	    bool is_http_only = false,
		const std::string& same_site = ""
	) final;
	const collections::Dictionary<std::string, Cookie>& get_cookies() final;
	void set_cookies(
		const collections::Dictionary<std::string, Cookie>& cookies
	) final;
	void delete_cookie(
		const std::string& name, const std::string& path, const std::string& domain
	) final;

	std::string get_reason_phrase() final;
	void set_reason_phrase(std::string value) final;

	unsigned short int status() final;
	std::string content_type() final;
	size_t content_length() override;
	std::string charset() final;
	bool is_streaming() final;

	// These methods partially implement the file-like object interface.
	void close() override;
	void write(const std::string& content) override;
	void flush() override;
	unsigned long int tell() override;

	/// These methods partially implement a stream-like object interface.
	bool readable() override;
	bool seekable() override;
	bool writable() override;
	void write_lines(const std::vector<std::string>& lines) override;

	Error err() final;
};


/// An HTTP response class with a string as content.
class HttpResponse : public HttpResponseBase
{
protected:
	std::string _content;

public:
	explicit HttpResponse(
		unsigned short int status = 200,
		const std::string& content = "",
		const std::string& content_type = "",
		const std::string& reason = "",
		const std::string& charset = "utf-8"
	);
	size_t content_length() override;
	void set_content(const std::string& content) override;
	std::string get_content() override;
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
