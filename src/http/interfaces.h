/**
 * http/interfaces.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Base libraries.
#include <xalwart.base/collections/dict.h>
#include <xalwart.base/result.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./cookie.h"


__HTTP_BEGIN__

class IHttpResponse
{
public:
	virtual ~IHttpResponse() = default;
	virtual std::string get_header(
		const std::string& key, const std::string& default_value
	) = 0;
	virtual void set_header(const std::string& key, const std::string& value) = 0;
	virtual void remove_header(const std::string& key) = 0;
	virtual bool has_header(const std::string& key) = 0;
	virtual void set_content(const std::string& content) = 0;
	virtual std::string get_content() = 0;

	virtual void set_cookie(
		const std::string& name,
		const std::string& value,
		long max_age = -1,
		const std::string& expires = "",
		const std::string& domain = "",
		const std::string& path = "/",
		bool is_secure = false,
		bool is_http_only = false,
		const std::string& same_site = ""
	) = 0;
	virtual void set_signed_cookie(
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
	) = 0;
	virtual const collections::Dict<std::string, Cookie>& get_cookies() = 0;
	virtual void set_cookies(
		const collections::Dict<std::string, Cookie>& cookies
	) = 0;
	virtual void delete_cookie(
		const std::string& name, const std::string& path, const std::string& domain
	) = 0;

	virtual std::string get_reason_phrase() = 0;
	virtual void set_reason_phrase(std::string value) = 0;

	virtual unsigned short int status() = 0;
	virtual std::string content_type() = 0;
	virtual size_t content_length() = 0;
	virtual std::string charset() = 0;
	virtual bool is_streaming() = 0;

	// These methods partially implement the file-like object interface.
	virtual void close() = 0;
	virtual void write(const std::string& content) = 0;
	virtual void flush() = 0;
	virtual unsigned long int tell() = 0;

	// These methods partially implement a stream-like object interface.
	virtual bool readable() = 0;
	virtual bool seekable() = 0;
	virtual bool writable() = 0;
	virtual void write_lines(const std::vector<std::string>& lines) = 0;

	virtual std::string serialize() = 0;

	virtual std::string& operator[] (const std::string& key) = 0;

	virtual Error err() = 0;
};

__HTTP_END__
