/**
 * http/request.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./request.h"

// Base libraries.
#include <xalwart.base/string_utils.h>
#include <xalwart.base/net/meta.h>

// Framework libraries.
#include "./exceptions.h"
#include "./mime/media_type.h"


__HTTP_BEGIN__

std::unique_ptr<mime::multipart::BodyReader> Request::multipart_reader(bool allow_mixed) const
{
	auto content_type = this->get_header(CONTENT_TYPE, "");
	if (content_type.empty())
	{
		throw exc::NotMultipart("request content is not 'multipart/form-data'", _ERROR_DETAILS_);
	}

	auto [media_type, parameters, ok] = mime::parse_media_type(str::string_to_wstring(content_type));
	if (!ok || !(media_type == L"multipart/form-data" || allow_mixed && media_type == L"multipart/mixed"))
	{
		throw exc::NotMultipart("request content is not 'multipart/form-data'", _ERROR_DETAILS_);
	}

	if (!parameters.contains(L"boundary"))
	{
		throw exc::HttpError(400, "missing start boundary", _ERROR_DETAILS_);
	}

	auto boundary = str::wstring_to_string(parameters.at(L"boundary"));
	return std::make_unique<mime::multipart::BodyReader>(
		this->body, boundary, std::stoll(this->get_header(CONTENT_LENGTH, "0")),
		this->max_file_upload_size, this->max_fields_count, this->max_header_length, this->max_headers_count
	);
}

std::string Request::get_raw_host(
	bool use_x_forwarded_host, bool use_x_forwarded_port,
	const std::optional<conf::Secure::Header>& secure_proxy_ssl_header
) const
{
	std::string raw_host;
	if (use_x_forwarded_host && this->header.contains(http::X_FORWARDED_HOST))
	{
		raw_host = this->header.at(http::X_FORWARDED_HOST);
	}
	else
	{
		if (!this->environment.contains(net::meta::SERVER_NAME))
		{
			throw KeyError("'environment' does not contain " + std::string(net::meta::SERVER_NAME));
		}

		raw_host = this->environment.at(net::meta::SERVER_NAME);
		auto port = this->get_port(use_x_forwarded_port);
		if (port != (this->is_secure(secure_proxy_ssl_header) ? "443" : "80"))
		{
			raw_host = raw_host + ":" + port;
		}
	}

	return raw_host;
}

std::string Request::get_port(bool use_x_forwarded_port) const
{
	std::string port;
	if (use_x_forwarded_port && this->header.contains(http::X_FORWARDED_PORT))
	{
		port = this->header.at(http::X_FORWARDED_PORT);
	}
	else
	{
		if (!this->environment.contains(net::meta::SERVER_PORT))
		{
			throw KeyError("'environment' does not contain " + std::string(net::meta::SERVER_PORT));
		}

		port = this->environment.at(net::meta::SERVER_PORT);
	}

	return port;
}

Request::Request(
	const net::RequestContext& context,
	ssize_t max_file_upload_size, ssize_t max_fields_count,
	ssize_t max_header_length, ssize_t max_headers_count,
	std::map<std::string, std::string> environment
) : method(context.method), url(std::move(parse_url(context.path + (context.query.empty() ? "" : "?" + context.query)))),
	proto("HTTP/" + std::to_string(context.protocol_version.major) + "." + std::to_string(context.protocol_version.minor)),
	proto_major((int)context.protocol_version.major), proto_minor((int)context.protocol_version.minor),
	header(context.headers), body(context.body), content_length((ssize_t)context.content_size),
	max_file_upload_size(max_file_upload_size), max_fields_count(max_fields_count),
	max_headers_count(max_headers_count), max_header_length(max_header_length),
	environment(std::move(environment))
{
	if (!valid_method(this->method))
	{
		throw ArgumentError("invalid method: '" + this->method + "'", _ERROR_DETAILS_);
	}

	this->url.host = remove_empty_port(this->url.host);
	this->host = this->url.host;
}

void Request::parse_form()
{
	if (!this->post_form)
	{
		if (this->method == "POST" || this->method == "PUT" || this->method == "PATCH")
		{
			auto parsed_query = parse_post_form(this);
			this->post_form = std::make_unique<Query>(parsed_query);
		}

		if (!this->post_form)
		{
			this->post_form = std::make_unique<Query>();
		}
	}

	if (!this->form)
	{
		if (!this->post_form->empty())
		{
			this->form = std::make_unique<Query>(*this->post_form);
		}

		auto new_query = std::make_unique<Query>(parse_query(this->url.raw_query));
		if (!this->form)
		{
			this->form = std::move(new_query);
		}
		else
		{
			for (auto it = new_query->begin(); it != new_query->end(); it++)
			{
				this->form->add(it->first, it->second);
			}
		}
	}
}

void Request::parse_multipart_form(long long int max_memory)
{
	if (!this->form)
	{
		this->parse_form();
	}

	if (this->multipart_form)
	{
		return;
	}

	auto reader = this->multipart_reader(false);
	auto target_form = reader->read_form(max_memory);
	if (!this->post_form)
	{
		this->post_form = std::make_unique<Query>();
	}

	for (const auto& pair : target_form->values)
	{
		this->form->add(pair.first, pair.second);

		// 'post_form' should also be populated.
		this->post_form->add(pair.first, pair.second);
	}

	this->multipart_form = std::move(target_form);
}

std::string Request::scheme(const std::optional<conf::Secure::Header>& secure_proxy_ssl_header) const
{
	if (secure_proxy_ssl_header.has_value())
	{
		auto header_val = this->get_header(secure_proxy_ssl_header->name, "");
		if (!header_val.empty())
		{
			return header_val == secure_proxy_ssl_header->value ? "https" : "http";
		}
	}

	return "http";
}

std::string Request::get_host(
	const std::optional<conf::Secure::Header>& secure_proxy_ssl_header,
	bool use_x_forwarded_host, bool use_x_forwarded_port, bool debug, std::vector<std::string> allowed_hosts
)
{
	auto raw_host = this->get_raw_host(use_x_forwarded_host, use_x_forwarded_port, secure_proxy_ssl_header);
	if (debug && allowed_hosts.empty())
	{
		allowed_hosts = {".localhost", "127.0.0.1", "::1"};
	}

	std::string domain, port;
	split_domain_port(raw_host, domain, port);
	if (!domain.empty() && validate_host(domain, allowed_hosts))
	{
		return raw_host;
	}

	auto msg = "Invalid HTTP_HOST header: ." + raw_host + ".";
	if (!domain.empty())
	{
		msg += " You may need to add " + domain + " to ALLOWED_HOSTS.";
	}
	else
	{
		msg += " The domain name provided is not valid according to RFC 1034/1035.";
	}

	throw exc::DisallowedHost(msg, _ERROR_DETAILS_);
}

bool has_port(const std::string& host)
{
	auto semicolon_idx = host.find_last_of(':');
	if (semicolon_idx == std::string::npos)
	{
		semicolon_idx = -1;
	}

	auto right_sq_bracket_idx = host.find_last_of(']');
	if (right_sq_bracket_idx == std::string::npos)
	{
		right_sq_bracket_idx = -1;
	}

	return semicolon_idx > right_sq_bracket_idx;
}

std::string remove_empty_port(const std::string& host)
{
	if (has_port(host))
	{
		return str::rtrim(host, ':');
	}

	return host;
}

void read_full_request_body(std::string& buffer, io::IReader* reader, ssize_t content_length)
{
	if (!reader)
	{
		throw NullPointerException("'reader' is nullptr", _ERROR_DETAILS_);
	}

	if (content_length < 0)
	{
		throw ArgumentError(
			"'content_length' is less then zero: " + std::to_string(content_length), _ERROR_DETAILS_
		);
	}
	else if (content_length == 0)
	{
	}
	else
	{
		for (size_t i = 0; content_length > 0; i++)
		{
			auto amount_of_bytes = reader->read(buffer, content_length);
			if (amount_of_bytes < 0)
			{
				throw ReaderError("read invalid amount of bytes: " + std::to_string(amount_of_bytes), _ERROR_DETAILS_);
			}

			content_length -= amount_of_bytes;
		}
	}
}

Query parse_post_form(Request* request)
{
	if (!request)
	{
		throw NullPointerException("'request' is nullptr", _ERROR_DETAILS_);
	}

	if (!request->body)
	{
		throw exc::HttpError(400, "missing form body", _ERROR_DETAILS_);
	}

	auto content_type = request->header.contains(CONTENT_TYPE) ? request->header.at(CONTENT_TYPE) : "";

	// RFC 7231, section 3.1.1.5 - empty type
	// MAY be treated as application/octet-stream
	if (content_type.empty())
	{
		content_type = "application/octet-stream";
	}

	Query query;
	auto [ct, _, ok] = mime::parse_media_type(str::string_to_wstring(content_type));
	if (ct == L"application/x-www-form-urlencoded")
	{
		auto content_length_string = request->get_header(CONTENT_LENGTH, "");
		if (!content_length_string.empty())
		{
			auto content_length = std::stoll(content_length_string);
			const long long int MAX_FORM_LENGTH = (long long int)1 << (63 - 1);
			if (content_length > MAX_FORM_LENGTH)
			{
				throw exc::PayloadTooLarge(
					"form body is too large: " + std::to_string(content_length), _ERROR_DETAILS_
				);
			}

			std::string buffer;
			read_full_request_body(buffer, request->body.get(), content_length);
			query = parse_query(buffer);
		}
	}
	else if (ct == L"multipart/form-data")
	{
		// handled by 'parse_multipart_form' (which is calling us, or should be).
	}

	return query;
}

__HTTP_END__
