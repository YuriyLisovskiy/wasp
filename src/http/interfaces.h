/**
 * http/interfaces.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Interfaces for http components.
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>
#include <vector>
#include <optional>

// Base libraries.
#include <xalwart.base/vendor/nlohmann/json.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./url.h"
#include "./mime/multipart/form.h"
#include "./cookie/cookie.h"
#include "../conf/types.h"


__HTTP_BEGIN__

class IRequest
{
public:
	// "HTTP/1.0"
	struct Proto
	{
		std::string name;     // HTTP
		unsigned short major; // 1
		unsigned short minor; // 0

		// 'at_least' reports whether the HTTP protocol used
		// in the request is at least major.minor.
		[[nodiscard]]
		inline bool at_least(int major_number, int minor_number) const
		{
			return this->major > major_number || this->major == major_number && this->minor >= minor_number;
		}

		[[nodiscard]]
		std::string to_string() const
		{
			return name + "/" + std::to_string(this->major) + "." + std::to_string(this->minor);
		}
	};

	[[nodiscard]]
	virtual std::string method() const = 0;

	[[nodiscard]]
	virtual const URL& url() const = 0;

	[[nodiscard]]
	virtual Proto proto() const = 0;

	[[nodiscard]]
	virtual const std::map<std::string, std::string>& headers() const = 0;

	// 'user_agent' returns the client's User-Agent, if sent in the request.
	[[nodiscard]]
	virtual std::string user_agent() const = 0;

	// TODO: docs for 'has_header'
	[[nodiscard]]
	virtual bool has_header(const std::string& key) const = 0;

	// TODO: docs for 'get_header'
	[[nodiscard]]
	virtual std::string get_header(const std::string& key, const std::string& default_value) const = 0;

	// TODO: docs for 'set_header'
	virtual void set_header(const std::string& key, const std::string& value) = 0;

	[[nodiscard]]
	virtual bool is_json() const = 0;

	// 'cookies' parses and returns the HTTP cookies sent with the request.
	[[nodiscard]]
	virtual std::vector<Cookie> cookies() const = 0;

	// 'cookie' returns the named cookie provided in the request or
	// 'nullptr' if not found.
	// If multiple cookies match the given name, only one cookie will
	// be returned.
	[[nodiscard]]
	virtual std::optional<Cookie> cookie(const std::string& name) const = 0;

	// 'referer' returns the referring URL, if sent in the request.
	//
	// 'referer' is misspelled as in the request itself, a mistake from the
	// earliest days of HTTP.  This value can also be fetched from the
	// 'header' dictionary as header.get("Referer"); the benefit of making it available
	// as a method is that the compiler can diagnose programs that use the
	// alternate (correct English) spelling req.referrer() but cannot
	// diagnose programs that use header.get("Referrer").
	[[nodiscard]]
	virtual std::string referer() const = 0;

	virtual const Query& form() = 0;

	virtual const mime::multipart::Form& multipart_form() = 0;

	virtual nlohmann::json json() = 0;

	[[nodiscard]]
	virtual std::string host() const = 0;

	[[nodiscard]]
	virtual ssize_t content_length() const = 0;

	[[nodiscard]]
	virtual const std::vector<std::string>& transfer_encoding() const = 0;

	[[nodiscard]]
	virtual const std::map<std::string, std::string>& environment() const = 0;

	// TODO: docs for 'scheme'
	[[nodiscard]]
	virtual std::string scheme(
		const std::optional<conf::Secure::Header>& secure_proxy_ssl_header
	) const = 0;

	// Return the HTTP host using the environment or request headers.
	virtual std::string get_host(
		const std::optional<conf::Secure::Header>& secure_proxy_ssl_header,
		bool use_x_forwarded_host, bool use_x_forwarded_port,
		bool debug, std::vector<std::string> allowed_hosts
	) = 0;

	[[nodiscard]]
	virtual bool is_secure(const std::optional<conf::Secure::Header>& secure_proxy_ssl_header) const = 0;
};

// TODO: docs for 'IResponse'
class IResponse
{
public:
	virtual ~IResponse() = default;

	[[nodiscard]]
	virtual std::string get_header(const std::string& key, const std::string& default_value) const = 0;

	virtual void set_header(const std::string& key, const std::string& value) = 0;

	virtual void remove_header(const std::string& key) = 0;

	[[nodiscard]]
	virtual bool has_header(const std::string& key) const = 0;

	virtual void set_content(const std::string& content) = 0;

	[[nodiscard]]
	virtual std::string get_content() const = 0;

	virtual void set_cookie(const Cookie& cookie) = 0;

	virtual void set_signed_cookie(const std::string& secret_key, const std::string& salt, const Cookie& cookie) = 0;

	[[nodiscard]]
	virtual const std::map<std::string, Cookie>& get_cookies() const = 0;

	virtual void set_cookies(const std::map<std::string, Cookie>& cookies) = 0;

	virtual void delete_cookie(const std::string& name, const std::string& path, const std::string& domain) = 0;

	[[nodiscard]]
	virtual std::string get_reason_phrase() const = 0;

	virtual void set_reason_phrase(const std::string& value) = 0;

	[[nodiscard]]
	virtual unsigned short int get_status() const = 0;

	[[nodiscard]]
	virtual std::string content_type() const = 0;

	[[nodiscard]]
	virtual size_t content_length() const = 0;

	[[nodiscard]]
	virtual std::string get_charset() const = 0;

	virtual void set_charset(const std::string& value) = 0;

	[[nodiscard]]
	virtual bool is_streaming() const = 0;

	virtual void close() = 0;

	virtual void write(const std::string& content) = 0;

	[[nodiscard]]
	virtual bool writable() const = 0;

	[[nodiscard]]
	virtual std::string serialize() = 0;
};

__HTTP_END__
