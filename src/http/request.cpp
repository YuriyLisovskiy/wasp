/**
 * http/request.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./request.h"

// Core libraries.
#include <xalwart.core/exceptions.h>
#include <xalwart.core/string_utils.h>
#include <xalwart.core/net/meta.h>

// Framework libraries.
#include "./headers.h"
#include "./utility.h"
#include "../conf/settings.h"


__HTTP_BEGIN__

HttpRequest::HttpRequest(
	const conf::Settings* settings,
	std::string method, std::string path, size_t major_v, size_t minor_v,
	std::string query, bool keep_alive, std::string content,
	collections::Dict<std::string, std::string> headers,
	HttpRequest::Parameters<std::string, std::string> get_params,
	HttpRequest::Parameters<std::string, std::string> post_params,
	HttpRequest::Parameters<std::string, files::UploadedFile> files_params,
	collections::Dict<std::string, std::string> env
) : settings(settings),
	_method(std::move(method)),
	_path(std::move(path)),
	_major_version(major_v),
	_minor_version(minor_v),
	_query(std::move(query)),
	_keep_alive(keep_alive),
	_body(std::move(content)),
	headers(std::move(headers)),
	GET(std::move(get_params)),
	POST(std::move(post_params)),
	FILES(std::move(files_params)),
	META(std::move(env))
{
}

std::string HttpRequest::version() const
{
	return std::to_string(this->_major_version) + "." + std::to_string(this->_minor_version);
}

std::string HttpRequest::path() const
{
	return this->_path;
}

std::string HttpRequest::full_path(bool force_append_slash) const
{
	return this->_path +
	       (force_append_slash && !this->_path.ends_with("/") ? "/" : "") +
	       (this->_query.empty() ? "" : "?" + this->_query);
}

std::string HttpRequest::query() const
{
	return this->_query;
}

std::string HttpRequest::method() const
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

core::Result<std::string> HttpRequest::get_host(
	bool use_x_forwarded_host, bool use_x_forwarded_port,
	bool debug, std::vector<std::string> allowed_hosts
)
{
	auto host = this->get_raw_host(use_x_forwarded_host, use_x_forwarded_port);
	if (debug && allowed_hosts.empty())
	{
		allowed_hosts = {".localhost", "127.0.0.1", "::1"};
	}

	std::string domain, port;
	split_domain_port(host, domain, port);
	if (!domain.empty() && validate_host(domain, allowed_hosts))
	{
		return core::Result(host);
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

		return core::raise<core::DisallowedHost, std::string>(msg);
	}
}

std::string HttpRequest::get_raw_host(
	bool use_x_forwarded_host, bool use_x_forwarded_port
) const
{
	std::string host;
	if (use_x_forwarded_host && this->headers.contains(http::X_FORWARDED_HOST))
	{
		host = this->headers.get(http::X_FORWARDED_HOST);
	}
	else if (this->META.contains("HTTP_HOST"))
	{
		host = this->META.get("HTTP_HOST");
	}
	else
	{
		host = this->META.get(net::meta::SERVER_NAME);
		auto port = this->get_port(use_x_forwarded_port);
		if (port != (this->is_secure(this->settings->SECURE_PROXY_SSL_HEADER.get()) ? "443" : "80"))
		{
			host = host + ":" + port;
		}
	}

	return host;
}

std::string HttpRequest::get_port(bool use_x_forwarded_port) const
{
	std::string port;
	if (use_x_forwarded_port && this->headers.contains(http::X_FORWARDED_PORT))
	{
		port = this->headers.get(http::X_FORWARDED_PORT);
	}
	else
	{
		port = this->META.get(net::meta::SERVER_PORT);
	}

	return port;
}

__HTTP_END__
