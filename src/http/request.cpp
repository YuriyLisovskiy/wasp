/**
 * http/request.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./request.h"

// Core libraries.
#include <xalwart.core/exceptions.h>
#include <xalwart.core/string_utils.h>

// Framework libraries.
#include "./headers.h"
#include "./utility.h"


__HTTP_BEGIN__

HttpRequest::HttpRequest(
	const std::string& method, std::string path, size_t major_v, size_t minor_v,
	std::string query, bool keep_alive, std::string content,
	const std::map<std::string, std::string>& headers,
	const HttpRequest::Parameters<std::string, std::string>& get_params,
	const HttpRequest::Parameters<std::string, std::string>& post_params,
	const HttpRequest::Parameters<std::string, core::UploadedFile>& files_params
)
:   _path(std::move(path)),
	_major_version(major_v),
	_minor_version(minor_v),
	_query(std::move(query)),
	_keep_alive(keep_alive),
	_body(std::move(content))
{
	this->_method = core::str::upper(method);
	this->headers = collections::Dict<std::string, std::string>(headers);
	this->GET = get_params;
	this->POST = post_params;
	this->FILES = files_params;
}

std::string HttpRequest::version() const
{
	return std::to_string(this->_major_version) + "." + std::to_string(this->_minor_version);
}

std::string HttpRequest::path()
{
	return this->_path;
}

std::string HttpRequest::full_path(bool force_append_slash)
{
	return this->_path +
		(force_append_slash && !core::str::ends_with(this->_path, "/") ? "/" : "") +
		(this->_query.empty() ? "" : "?" + this->_query);
}

std::string HttpRequest::query()
{
	return this->_query;
}

std::string HttpRequest::method()
{
	return this->_method;
}

bool HttpRequest::keep_alive() const
{
	return this->_keep_alive;
}

std::string HttpRequest::body()
{
	return this->_body;
}

bool HttpRequest::is_secure(
	std::pair<std::string, std::string>* secure_proxy_ssl_header
) const
{
	return this->scheme(secure_proxy_ssl_header) == "https";
}

std::string HttpRequest::scheme(
	std::pair<std::string, std::string>* secure_proxy_ssl_header
) const
{
	if (secure_proxy_ssl_header)
	{
		auto header_val = this->headers.get(
			secure_proxy_ssl_header->first, ""
		);
		if (!header_val.empty())
		{
			return header_val == secure_proxy_ssl_header->second ? "https" : "http";
		}
	}

	// Default scheme.
	return "http";
}

Result<std::string> HttpRequest::get_host(
	bool use_x_forwarded_host, bool debug, std::vector<std::string> allowed_hosts
)
{
	auto host = this->get_raw_host(use_x_forwarded_host);
	if (debug && allowed_hosts.empty())
	{
		allowed_hosts = {".localhost", "127.0.0.1", "::1"};
	}

	std::string domain, port;
	split_domain_port(host, domain, port);
	if (!domain.empty() && validate_host(domain, allowed_hosts))
	{
		return Result(host);
	}
	else
	{
		auto msg = "Invalid HTTP_HOST header: ." + host + ".";
		if (!domain.empty())
		{
			msg += " You may need to add " + domain + " to ALLOWED_HOSTS.";
		}
		else
		{
			msg += " The domain name provided is not valid according to RFC 1034/1035.";
		}

		return raise<DisallowedHost, std::string>(msg);
	}
}

std::string HttpRequest::get_raw_host(bool use_x_forwarded_host) const
{
	std::string host;
	if (use_x_forwarded_host && this->headers.contains(http::X_FORWARDED_HOST))
	{
		host = this->headers.get(http::X_FORWARDED_HOST);
	}
	else
	{
		host = this->headers.get(http::HOST);
	}

	return host;
}

__HTTP_END__
