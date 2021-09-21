/**
 * http/abc.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>
#include <vector>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./cookie/cookie.h"


__HTTP_ABC_BEGIN__

// TODO: docs for 'HttpResponse'
class HttpResponse
{
public:
	virtual ~HttpResponse() = default;

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

__HTTP_ABC_END__
