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
#include <xalwart.base/vendor/nlohmann/json.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./interfaces.h"
#include "./headers.h"
#include "./exceptions.h"
#include "./mime/content_types.h"


__HTTP_BEGIN__

// TESTME: ResponseBase
// TODO: docs for 'ResponseBase'
// An HTTP response base class with dictionary-accessed headers.
class AbstractResponse : public HttpResponse
{
public:
	explicit AbstractResponse(
		unsigned short int status, std::string content_type, std::string reason, std::string charset
	);

	~AbstractResponse() override = default;

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
		throw RuntimeError("Response instance is not writable", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	inline bool writable() const override
	{
		return false;
	}

protected:
	std::map<std::string, std::string> headers;
	std::map<std::string, Cookie> cookies;
	bool closed;
	unsigned short int status;
	std::string charset;
	std::string reason_phrase;
	bool streaming;

	[[nodiscard]]
	std::string serialize_headers() const;
};

// TESTME: Response
// TODO: docs for 'Response'
// A non-streaming HTTP response class which implements basic
// functionality based on derived classes which implements content.
class BaseResponse : public AbstractResponse
{
public:
	explicit BaseResponse(
		unsigned short int status, const std::string& content_type,
		const std::string& reason, const std::string& charset
	) : AbstractResponse(status, content_type, reason, charset)
	{
		this->streaming = false;
	}

	[[nodiscard]]
	inline size_t content_length() const override
	{
		return this->get_content().size();
	}

	inline void set_content(const std::string& content) override
	{
		throw NotImplementedException("'set_content(const std::string&)' is not implemented", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	inline std::string get_content() const override
	{
		throw NotImplementedException("'get_content()' is not implemented", _ERROR_DETAILS_);
	}

	inline void write(const std::string& data) override
	{
		throw NotImplementedException("'write(const std::string&)' is not implemented", _ERROR_DETAILS_);
	}

	[[nodiscard]]
	inline bool writable() const override
	{
		return true;
	}

	std::string serialize() override;
};

// TESTME: Response
// TODO: docs for 'Response'
// An HTTP response class with a string as content.
class Response : public BaseResponse
{
public:
	explicit Response(
		std::string content,
		unsigned short int status=200,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	) : BaseResponse(status, content_type, reason, charset)
	{
		this->content = std::move(content);
	}

	explicit Response(
		unsigned short int status=200,
		const std::string& content="",
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	) : Response(content, status, content_type, reason, charset)
	{
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

protected:
	std::string content;
};

// TESTME: JsonResponse
// TODO: docs for 'JsonResponse'
// An HTTP response class with JSON content.
class JsonResponse final : public BaseResponse
{
public:
	explicit JsonResponse(
		nlohmann::json data,
		unsigned short int status=200,
		const std::string& reason="",
		const std::string& charset=""
	) : BaseResponse(status, mime::APPLICATION_JSON, reason, charset)
	{
		this->json_content = std::move(data);
	}

	explicit JsonResponse(
		const std::string& data,
		unsigned short int status=200,
		const std::string& reason="",
		const std::string& charset=""
	) : JsonResponse(std::move(nlohmann::json::parse(data)), status, reason, charset)
	{
	}

	inline void set_content(const std::string& content) override
	{
		this->json_content = nlohmann::json::parse(content);
	}

	inline void set_content(const nlohmann::json& data)
	{
		this->json_content = data;
	}

	[[nodiscard]]
	inline std::string get_content() const override
	{
		return this->json_content.dump();
	}

	[[nodiscard]]
	inline nlohmann::json get_json_content() const
	{
		return this->json_content;
	}

	// Converts `json_content` into JSON array if it is not done
	// already and parses and appends `data` to `json_content`.
	inline void write(const std::string& data) override
	{
		this->write(nlohmann::json::parse(data));
	}

	inline void write(const nlohmann::json& data)
	{
		if (!this->json_content.is_array())
		{
			this->json_content = nlohmann::json::array({this->json_content});
		}

		this->json_content.push_back(data);
	}

protected:
	nlohmann::json json_content;
};

// TESTME: StreamingResponse
// TODO: docs for 'StreamingResponse'
class StreamingResponse : public AbstractResponse
{
public:
	inline explicit StreamingResponse(
		unsigned short int status=0,
		const std::string& content_type="",
		const std::string& reason="",
		const std::string& charset=""
	) : AbstractResponse(status, content_type, reason, charset)
	{
		this->streaming = true;
	}

	[[nodiscard]]
	inline size_t content_length() const final
	{
		return 0;
	}

	inline void set_content(const std::string& new_content) final
	{
	}

	[[nodiscard]]
	inline std::string get_content() const final
	{
		return "";
	}

	[[nodiscard]]
	virtual unsigned long int tell() const = 0;

	virtual void flush() = 0;

	[[nodiscard]]
	virtual bool readable() const = 0;

	[[nodiscard]]
	virtual bool seekable() const = 0;

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

	inline void flush() override
	{
	}

	[[nodiscard]]
	inline bool readable() const override
	{
		return false;
	}

	[[nodiscard]]
	inline bool seekable() const override
	{
		return false;
	}

	[[nodiscard]]
	inline unsigned long int tell() const override
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
	static inline const std::set<std::string> ALLOWED_SCHEMES = {"http", "https", "ftp"};
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
	) : Response(content, 304, content_type, "", charset)
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
	) : Response(content, 400, content_type, "", charset)
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
	) : Response(content, 404, content_type, "", charset)
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
	) : Response(content, 403, content_type, "", charset)
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
	) : Response(content, 410, content_type, "", charset)
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
	) : Response(content, 413, content_type, "", charset)
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
	) : Response(content, 500, content_type, "", charset)
	{
	}
};

__HTTP_END__
