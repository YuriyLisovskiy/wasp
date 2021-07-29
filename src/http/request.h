/**
 * http/request.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: represents an http request.
 */

#pragma once

// Base libraries.
#include <xalwart.base/collections/dictionary.h>
#include <xalwart.base/collections/multi_dictionary.h>
#include <xalwart.base/result.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/_def_.h"
#include "../core/uploaded_file.h"


__CONF_BEGIN__
struct Settings;
__CONF_END__


__HTTP_BEGIN__

class HttpRequest
{
public:
	explicit HttpRequest(
		const conf::Settings* settings,
		std::string method, std::string path, size_t major_v, size_t minor_v,
		std::string query, bool keep_alive, std::string content,
		collections::Dictionary<std::string, std::string> headers,
		collections::MultiDictionary<std::string, std::string> get_params,
		collections::MultiDictionary<std::string, std::string> post_params,
		collections::MultiDictionary<std::string, files::UploadedFile> files_params,
		collections::Dictionary<std::string, std::string> meta_params
	);

	collections::Dictionary<std::string, std::string> headers;
	collections::MultiDictionary<std::string, std::string> GET;
	collections::MultiDictionary<std::string, std::string> POST;
	collections::Dictionary<std::string, std::string> COOKIES;
	collections::MultiDictionary<std::string, files::UploadedFile> FILES;
	collections::Dictionary<std::string, std::string> META;

	[[nodiscard]]
	std::string version() const;

	[[nodiscard]]
	std::string path() const;

	[[nodiscard]]
	std::string full_path(bool force_append_slash = false) const;

	[[nodiscard]]
	std::string query() const;

	[[nodiscard]]
	std::string method() const;

	[[nodiscard]]
	bool keep_alive() const;

	std::string body();

	bool is_secure(std::pair<std::string, std::string>* secure_proxy_ssl_header) const;

	std::string scheme(std::pair<std::string, std::string>* secure_proxy_ssl_header) const;

	/// Return the HTTP host using the environment or request headers.
	Result<std::string> get_host(
		bool use_x_forwarded_host, bool use_x_forwarded_port,
		bool debug, std::vector<std::string> allowed_hosts
	);

protected:
	// Return the HTTP host using the environment or request headers. Skip
	// allowed hosts protection, so may return an insecure host.
	[[nodiscard]]
	std::string get_raw_host(
		bool use_x_forwarded_host, bool use_x_forwarded_port
	) const;

	[[nodiscard]]
	std::string get_port(bool use_x_forwarded_port) const;

protected:
	const conf::Settings* settings;

private:
	size_t _major_version;
	size_t _minor_version;
	std::string _path;
	std::string _query;
	std::string _method;
	bool _keep_alive;
	std::string _body;
};

__HTTP_END__
