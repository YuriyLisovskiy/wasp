/**
 * http/request.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Http request representation.
 */

#pragma once

// C++ libraries.
#include <memory>

// Base libraries.
#include <xalwart.base/collections/dictionary.h>
#include <xalwart.base/collections/multi_dictionary.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./url.h"
#include "../conf/_def_.h"
#include "../core/uploaded_file.h"


__CONF_BEGIN__

struct Settings;

__CONF_END__


__HTTP_BEGIN__

// TESTME: Request
// TODO: docs for 'Request'
class Request
{
private:
	size_t _major_version;
	size_t _minor_version;
	std::string _path;
	std::string _raw_query;
	std::string _method;
	bool _keep_alive;
	std::string _body;

protected:
	const conf::Settings* settings;

protected:
	// Return the HTTP host using the environment or request headers. Skip
	// allowed hosts protection, so may return an insecure host.
	[[nodiscard]]
	std::string get_raw_host(bool use_x_forwarded_host, bool use_x_forwarded_port) const;

	[[nodiscard]]
	std::string get_port(bool use_x_forwarded_port) const;

public:
	inline explicit Request(
		const conf::Settings* settings,
		std::string method, std::string path, size_t major_v, size_t minor_v,
		std::string raw_query, bool keep_alive, std::string content,
		collections::Dictionary<std::string, std::string> headers,
		collections::MultiDictionary<std::string, std::string> get_params,
		collections::MultiDictionary<std::string, std::string> post_params,
		collections::MultiDictionary<std::string, files::UploadedFile> files_params,
		collections::Dictionary<std::string, std::string> env
	) : settings(settings),
		_method(std::move(method)),
		_path(std::move(path)),
		_major_version(major_v),
		_minor_version(minor_v),
		_raw_query(std::move(raw_query)),
		_keep_alive(keep_alive),
		_body(std::move(content)),
		headers(std::move(headers)),
		GET(std::move(get_params)),
		POST(std::move(post_params)),
		FILES(std::move(files_params)),
		META(std::move(env))
	{
	}

	collections::Dictionary<std::string, std::string> headers;
	collections::MultiDictionary<std::string, std::string> GET;
	collections::MultiDictionary<std::string, std::string> POST;
	collections::Dictionary<std::string, std::string> COOKIES;
	collections::MultiDictionary<std::string, files::UploadedFile> FILES;
	collections::Dictionary<std::string, std::string> META;

	[[nodiscard]]
	inline std::string protocol_version() const
	{
		return std::to_string(this->_major_version) + "." + std::to_string(this->_minor_version);
	}

	[[nodiscard]]
	inline std::string path() const
	{
		return this->_path;
	}

	[[nodiscard]]
	inline std::string full_path(bool force_append_slash=false) const
	{
		return this->_path + (force_append_slash && !this->_path.ends_with("/") ? "/" : "") +
			(this->_raw_query.empty() ? "" : "?" + this->_raw_query);
	}

	[[nodiscard]]
	inline query_t query() const
	{
		return parse_query(this->_raw_query);
	}

	[[nodiscard]]
	inline std::string raw_query() const
	{
		return this->_raw_query;
	}

	[[nodiscard]]
	inline std::string method() const
	{
		return this->_method;
	}

	[[nodiscard]]
	inline bool keep_alive() const
	{
		return this->_keep_alive;
	}

	[[nodiscard]]
	inline std::string body() const
	{
		return this->_body;
	}

	[[nodiscard]]
	inline bool is_secure(std::pair<std::string, std::string>* secure_proxy_ssl_header) const
	{
		return this->scheme(secure_proxy_ssl_header) == "https";
	}

	std::string scheme(std::pair<std::string, std::string>* secure_proxy_ssl_header) const;

	// Return the HTTP host using the environment or request headers.
	std::pair<std::string, std::shared_ptr<BaseException>> get_host(
		bool use_x_forwarded_host, bool use_x_forwarded_port, bool debug, std::vector<std::string> allowed_hosts
	);
};

__HTTP_END__
