/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * An implementation of request.h.
 */

#include "./request.h"


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

std::string HttpRequest::version()
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

bool HttpRequest::keep_alive()
{
	return this->_keep_alive;
}

std::string HttpRequest::body()
{
	return this->_body;
}

bool HttpRequest::is_secure(
	std::pair<std::string, std::string>* secure_proxy_ssl_header
)
{
	return this->scheme(secure_proxy_ssl_header) == "https";
}

std::string HttpRequest::scheme(
	std::pair<std::string, std::string>* secure_proxy_ssl_header
)
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

std::string HttpRequest::get_host(
	bool use_x_forwarded_host, bool debug, std::vector<std::string> allowed_hosts
)
{
	auto host = this->get_raw_host(use_x_forwarded_host);
	if (debug && allowed_hosts.empty())
	{
		allowed_hosts = {".localhost", "127.0.0.1", "[::1]"};
	}

	std::string domain, port;
	split_domain_port(host, domain, port);
	if (!domain.empty() && validate_host(domain, allowed_hosts))
	{
		return host;
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

		throw core::DisallowedHost(msg);
	}
}

std::string HttpRequest::get_raw_host(bool use_x_forwarded_host)
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
