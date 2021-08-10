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

// Base libraries.
#include <xalwart.base/collections/dictionary.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"


__HTTP_BEGIN__

// TESTME: ResponseBase
// TODO: docs for 'ResponseBase'
// An HTTP response base class with dictionary-accessed headers.
//
// This class doesn't handle content. It should not be used directly.
// Use the HttpResponse subclass instead.
class ResponseBase : public abc::IHttpResponse
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

protected:
	std::string serialize_headers();

public:
	explicit ResponseBase(
		unsigned short int status=0,
		std::string content_type="",
		const std::string& reason="",
		const std::string& charset="utf-8"
	);

	~ResponseBase() override = default;

	inline std::string get_header(const std::string& key, const std::string& default_value) final
	{
		return this->_headers.get(key, default_value);
	}

	inline void set_header(const std::string& key, const std::string& value) final
	{
		this->_headers.set(key, value);
	}

	inline void remove_header(const std::string& key) final
	{
		this->_headers.remove(key);
	}

	inline bool has_header(const std::string& key) final
	{
		return this->_headers.contains(key);
	}

	inline void set_content(const std::string& content) override
	{
	}

	inline std::string get_content() override
	{
		return "";
	}

	void set_cookie(
		const std::string& name,
		const std::string& value,
		long max_age=-1,
		const std::string& expires="",
		const std::string& domain="",
		const std::string& path="/",
		bool is_secure=false,
		bool is_http_only=false,
		const std::string& same_site=""
	) final;

	void set_signed_cookie(
		const std::string& secret_key,
		const std::string& name,
		const std::string& value,
		const std::string& salt="",
		long max_age=-1,
		const std::string& expires="",
		const std::string& domain="",
		const std::string& path="/",
		bool is_secure=false,
		bool is_http_only=false,
		const std::string& same_site=""
	) final;

	inline const collections::Dictionary<std::string, Cookie>& get_cookies() final
	{
		return this->_cookies;
	}

	inline void set_cookies(const collections::Dictionary<std::string, Cookie>& cookies) final
	{
		this->_cookies = cookies;
	}

	void delete_cookie(const std::string& name, const std::string& path, const std::string& domain) final;

	std::string get_reason_phrase() final;

	void set_reason_phrase(std::string value) final;

	inline unsigned short int status() final
	{
		return this->_status;
	}

	inline std::string content_type() final
	{
		return this->_headers.get("Content-Type", "");
	}

	inline size_t content_length() override
	{
		return 0;
	}

	inline std::string charset() final
	{
		return this->_charset;
	}

	inline bool is_streaming() final
	{
		return this->_streaming;
	}

	// These methods partially implement the file-like object interface.
	inline void close() override
	{
		this->_closed = true;
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
};

// TESTME: Response
// TODO: docs for 'Response'
// An HTTP response class with a string as content.
class Response : public ResponseBase
{
protected:
	std::string _content;

public:
	explicit Response(
		unsigned short int status=200,
		const std::string& content="",
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset="utf-8"
	) : ResponseBase(status, content_type, reason, charset)
	{
		this->_content = content;
		this->_streaming = false;
	}

	inline size_t content_length() override
	{
		return this->_content.size();
	}

	inline void set_content(const std::string& content) override
	{
		this->_content = content;
	}

	inline std::string get_content() override
	{
		return this->_content;
	}

	inline void write(const std::string& content) override
	{
		this->_content.append(content);
	}

	inline unsigned long int tell() override
	{
		return this->_content.size();
	}

	inline bool writable() override
	{
		return true;
	}

	void write_lines(const std::vector<std::string>& lines) override;

	std::string serialize() override;
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
		const std::string& charset="utf-8"
	) : ResponseBase(status, content_type, reason, charset)
	{
		this->_streaming = true;
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

private:
	void _set_headers();

	std::string _get_headers_chunk();

public:
	explicit FileResponse(
		std::string file_path,
		bool as_attachment=false,
		unsigned short int status=0,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset="utf-8"
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
};

struct result_t
{
	std::shared_ptr<abc::IHttpResponse> response = nullptr;
	std::shared_ptr<BaseException> exception = nullptr;
};

// TESTME: result<HttpResponseT, ...ArgsT>
// TODO: docs for 'result<HttpResponseT, ...ArgsT>'
template <typename HttpResponseT, typename ...ArgsT>
inline result_t result(ArgsT&& ...args)
{
	return result_t{std::make_shared<HttpResponseT>(std::forward<ArgsT>(args)...), nullptr};
}

// TESTME: error
// TODO: docs for 'error'
template <typename ExceptionT, typename ...ArgsT>
inline result_t exception(ArgsT&& ...args)
{
	return result_t{nullptr, std::make_shared<ExceptionT>(std::forward<ArgsT>(args)...)};
}

// TESTME: raise
// TODO: docs for 'raise'
template <typename ...ArgsT>
inline result_t raise(unsigned short int status_code, ArgsT&& ...args)
{
	return result_t{std::make_shared<http::Response>(status_code, std::forward<ArgsT...>(args)...), nullptr};
}

__HTTP_END__


__HTTP_RESP_BEGIN__

// TESTME: RedirectBase
// TODO: docs for 'RedirectBase'
class RedirectBase : public Response
{
protected:
	const std::set<std::string> _allowed_schemes = {"http", "https", "ftp"};

public:
	explicit RedirectBase(
		const std::string& redirect_to,
		unsigned short int status,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset="utf-8"
	);

	[[nodiscard]]
	inline std::string url() const
	{
		return this->_headers.get("Location");
	}
};

// TESTME: Redirect
// TODO: docs for 'Redirect'
class Redirect : public RedirectBase
{
public:
	inline explicit Redirect(
		const std::string& redirect_to,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& redirect_to,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	) : Response(304, content, content_type, "", charset)
	{
		this->remove_header("Content-Type");
	}

	void set_content(const std::string& content) override;
};

// TESTME: BadRequest
// TODO: docs for 'BadRequest'
class BadRequest : public Response
{
public:
	inline explicit BadRequest(
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::vector<std::string>& permitted_methods,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	);
};

// TESTME: Gone
// TODO: docs for 'Gone'
class Gone : public Response
{
public:
	inline explicit Gone(
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
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
		const std::string& content,
		const std::string& content_type="",
		const std::string& charset="utf-8"
	) : Response(500, content, content_type, "", charset)
	{
	}
};

__HTTP_RESP_END__
