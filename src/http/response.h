/**
 * http/response.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Contains base http response and most frequently used http response.
 */

#pragma once

// C++ libraries.
#include <set>
#include <fstream>
#include <memory>
#include <map>

// Base libraries.
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "./headers.h"
#include "exceptions.h"


__HTTP_BEGIN__

// TESTME: ResponseBase
// TODO: docs for 'ResponseBase'
// An HTTP response base class with dictionary-accessed headers.
//
// This class doesn't handle content. It should not be used directly.
// Use the HttpResponse subclass instead.
class ResponseBase : public abc::IHttpResponse
{
public:
	explicit ResponseBase(
		unsigned short int status=0, std::string content_type="", std::string reason="", std::string charset=""
	);

	~ResponseBase() override = default;

	[[nodiscard]]
	inline std::string get_header(const std::string& key, const std::string& default_value) const final
	{
		return this->has_header(key) ? this->headers.at(key) : default_value;
	}

	inline void set_header(const std::string& key, const std::string& value) final
	{
		if (this->has_header(key))
		{
			this->headers[key] = value;
		}

		this->headers.insert(std::make_pair(key, value));
	}

	inline void remove_header(const std::string& key) final
	{
		if (this->has_header(key))
		{
			this->headers.erase(key);
		}
	}

	[[nodiscard]]
	inline bool has_header(const std::string& key) const final
	{
		return this->headers.contains(key);
	}

	inline void set_content(const std::string& content) override
	{
	}

	[[nodiscard]]
	inline std::string get_content() const override
	{
		return "";
	}

	void set_cookie(const Cookie& cookie) final;

	void set_signed_cookie(const std::string& secret_key, const std::string& salt, const Cookie& cookie) final;

	[[nodiscard]]
	inline const std::map<std::string, Cookie>& get_cookies() const final
	{
		return this->cookies;
	}

	inline void set_cookies(const std::map<std::string, Cookie>& new_cookies) final
	{
		this->cookies = new_cookies;
	}

	void delete_cookie(const std::string& name, const std::string& path, const std::string& domain) final;

	[[nodiscard]]
	std::string get_reason_phrase() const final;

	inline void set_reason_phrase(const std::string& value) final
	{
		this->reason_phrase = value.empty() ? "Unknown Status" : value;
	}

	[[nodiscard]]
	inline unsigned short int get_status() const final
	{
		return this->status;
	}

	[[nodiscard]]
	inline std::string content_type() const final
	{
		return this->get_header(CONTENT_TYPE, "");
	}

	[[nodiscard]]
	inline size_t content_length() const override
	{
		return 0;
	}

	[[nodiscard]]
	inline std::string get_charset() const final
	{
		return this->charset;
	}

	inline void set_charset(const std::string& value) final
	{
		this->charset = value;
	}

	[[nodiscard]]
	inline bool is_streaming() const final
	{
		return this->streaming;
	}

	// These methods partially implement the file-like object interface.
	inline void close() override
	{
		this->closed = true;
	}

	inline void write(const std::string& content) override
	{
		throw RuntimeError("This 'xw::http::ResponseBase' instance is not writable", _ERROR_DETAILS_);
	}

	inline void flush() override
	{
	}

	inline unsigned long int tell() override
	{
		throw RuntimeError("This 'xw::http::ResponseBase' instance cannot tell its position", _ERROR_DETAILS_);
	}

	// These methods partially implement a stream-like object interface.
	inline bool readable() override
	{
		return false;
	}

	inline bool seekable() override
	{
		return false;
	}

	inline bool writable() override
	{
		return false;
	}

	inline void write_lines(const std::vector<std::string>& lines) override
	{
		throw RuntimeError("This 'xw::http::ResponseBase' instance is not writable", _ERROR_DETAILS_);
	}

protected:
	std::map<std::string, std::string> headers;
	std::map<std::string, Cookie> cookies;
	bool closed;
	unsigned short int status;
	std::string charset;
	std::string reason_phrase;
	bool streaming;

	std::string serialize_headers();
};

// TESTME: Response
// TODO: docs for 'Response'
// An HTTP response class with a string as content.
class Response : public ResponseBase
{
public:
	explicit Response(
		unsigned short int status=200,
		std::string content="",
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	) : ResponseBase(status, content_type, reason, charset)
	{
		this->content = std::move(content);
		this->streaming = false;
	}

	[[nodiscard]]
	inline size_t content_length() const override
	{
		return this->content.size();
	}

	inline void set_content(const std::string& new_content) override
	{
		this->content = new_content;
	}

	[[nodiscard]]
	inline std::string get_content() const override
	{
		return this->content;
	}

	inline void write(const std::string& data) override
	{
		this->content.append(data);
	}

	inline unsigned long int tell() override
	{
		return this->content.size();
	}

	inline bool writable() override
	{
		return true;
	}

	inline void write_lines(const std::vector<std::string>& lines) override
	{
		for (const auto & line : lines)
		{
			this->write(line);
		}
	}

	std::string serialize() override;

protected:
	std::string content;
};

// TESTME: StreamingResponse
// TODO: docs for 'StreamingResponse'
class StreamingResponse : public ResponseBase
{
public:
	inline explicit StreamingResponse(
		unsigned short int status=0,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	) : ResponseBase(status, content_type, reason, charset)
	{
		this->streaming = true;
	}

	inline std::string serialize() final
	{
		throw RuntimeError(
			"This 'xw::http::StreamingResponse' or its child instance cannot be serialized", _ERROR_DETAILS_
		);
	}

	virtual std::string get_chunk() = 0;
};

// TESTME: FileResponse
// TODO: docs for 'FileResponse'
class FileResponse final : public StreamingResponse
{
public:
	explicit FileResponse(
		std::string file_path,
		bool as_attachment=false,
		unsigned short int status=0,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	);

	std::string get_chunk() override;

	inline unsigned long int tell() override
	{
		return this->_bytes_read;
	}

	void close() override
	{
		StreamingResponse::close();
		this->_file_stream.close();
	}

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
};

__HTTP_END__


__HTTP_RESP_BEGIN__

// TESTME: RedirectBase
// TODO: docs for 'RedirectBase'
class RedirectBase : public Response
{
public:
	explicit RedirectBase(
		const std::string& redirect_to, unsigned short int status,
		const std::string& content_type="", const std::string& reason="",
		const std::string& charset=""
	);

	[[nodiscard]]
	inline std::string url() const
	{
		return this->get_header(LOCATION, "");
	}

protected:
	inline static const std::set<std::string> ALLOWED_SCHEMES = {"http", "https", "ftp"};
};

// TESTME: Redirect
// TODO: docs for 'Redirect'
class Redirect : public RedirectBase
{
public:
	inline explicit Redirect(
		const std::string& redirect_to, const std::string& content_type="", const std::string& charset=""
	) : RedirectBase(redirect_to, 302, content_type, "", charset)
	{
	}
};

// TESTME: PermanentRedirect
// TODO: docs for 'PermanentRedirect'
class PermanentRedirect : public RedirectBase
{
public:
	inline explicit PermanentRedirect(
		const std::string& redirect_to, const std::string& content_type="", const std::string& charset=""
	) : RedirectBase(redirect_to, 301, content_type, "", charset)
	{
	}
};

// TESTME: NotModified
// TODO: docs for 'NotModified'
class NotModified : public Response
{
public:
	inline explicit NotModified(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(304, content, content_type, "", charset)
	{
		this->remove_header(http::CONTENT_TYPE);
	}

	void set_content(const std::string& content) override;
};

// TESTME: BadRequest
// TODO: docs for 'BadRequest'
class BadRequest : public Response
{
public:
	inline explicit BadRequest(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(400, content, content_type, "", charset)
	{
	}
};

// TESTME: NotFound
// TODO: docs for 'NotFound'
class NotFound : public Response
{
public:
	inline explicit NotFound(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(404, content, content_type, "", charset)
	{
	}
};

// TESTME: Forbidden
// TODO: docs for 'Forbidden'
class Forbidden : public Response
{
public:
	inline explicit Forbidden(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(403, content, content_type, "", charset)
	{
	}
};

// TESTME: NotAllowed
// TODO: docs for 'NotAllowed'
class NotAllowed : public Response
{
public:
	explicit NotAllowed(
		const std::string& content, const std::vector<std::string>& permitted_methods,
		const std::string& content_type="", const std::string& charset=""
	);
};

// TESTME: Gone
// TODO: docs for 'Gone'
class Gone : public Response
{
public:
	inline explicit Gone(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(410, content, content_type, "", charset)
	{
	}
};

// TESTME: EntityTooLarge
// TODO: docs for 'EntityTooLarge'
class EntityTooLarge : public Response
{
public:
	inline explicit EntityTooLarge(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(413, content, content_type, "", charset)
	{
	}
};

// TESTME: ServerError
// TODO: docs for 'ServerError'
class ServerError : public Response
{
public:
	inline explicit ServerError(
		const std::string& content, const std::string& content_type="", const std::string& charset=""
	) : Response(500, content, content_type, "", charset)
	{
	}
};

__HTTP_RESP_END__
