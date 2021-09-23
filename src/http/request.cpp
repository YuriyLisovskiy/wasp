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

Request::Request(
	const net::RequestContext& context,
	ssize_t max_file_upload_size, ssize_t max_fields_count,
	ssize_t max_header_length, ssize_t max_headers_count,
	long long int multipart_max_memory,
	bool throw_on_invalid_content_type,
	std::map<std::string, std::string> environment
) : _method(context.method),
	_headers(context.headers), _body_reader(context.body), _content_length((ssize_t)context.content_size),
	max_file_upload_size(max_file_upload_size), max_fields_count(max_fields_count),
	max_headers_count(max_headers_count), max_header_length(max_header_length),
	multipart_max_memory(multipart_max_memory), _environment(std::move(environment)),
	throw_on_invalid_content_type(throw_on_invalid_content_type)
{
	if (!valid_method(this->_method))
	{
		throw ArgumentError("invalid method: '" + this->_method + "'", _ERROR_DETAILS_);
	}

	this->_proto = {
		.name = "HTTP",
		.major = context.protocol_version.major,
		.minor = context.protocol_version.minor
	};
	this->_url = std::move(parse_url(context.path + (context.query.empty() ? "" : "?" + context.query)));
	this->_url.host = remove_empty_port(this->_url.host);
	this->_host = this->_url.host;
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
	auto raw_host = this->_get_raw_host(use_x_forwarded_host, use_x_forwarded_port, secure_proxy_ssl_header);
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

std::unique_ptr<mime::multipart::BodyReader> Request::_multipart_reader(bool allow_mixed) const
{
	auto content_type = this->get_header(CONTENT_TYPE, "");
	if (content_type.empty())
	{
		throw exc::NotMultipart(
			"request content is not '" + std::string(mime::MULTIPART_FORM_DATA) + "'", _ERROR_DETAILS_
		);
	}

	auto [media_type, parameters, ok] = mime::parse_media_type(str::string_to_wstring(content_type));
	if (!ok || !(media_type == mime::MULTIPART_FORM_DATA_L || allow_mixed && media_type == L"multipart/mixed"))
	{
		throw exc::NotMultipart(
			"request content is not '" + std::string(mime::MULTIPART_FORM_DATA) + "'", _ERROR_DETAILS_
		);
	}

	if (!parameters.contains(L"boundary"))
	{
		throw exc::HttpError(400, "missing start boundary", _ERROR_DETAILS_);
	}

	auto boundary = str::wstring_to_string(parameters.at(L"boundary"));
	return std::make_unique<mime::multipart::BodyReader>(
		this->_body_reader, boundary, std::stoll(this->get_header(CONTENT_LENGTH, "0")),
		this->max_file_upload_size, this->max_fields_count, this->max_header_length, this->max_headers_count
	);
}

std::string Request::_get_raw_host(
	bool use_x_forwarded_host, bool use_x_forwarded_port,
	const std::optional<conf::Secure::Header>& secure_proxy_ssl_header
) const
{
	std::string raw_host;
	if (use_x_forwarded_host && this->_headers.contains(http::X_FORWARDED_HOST))
	{
		raw_host = this->_headers.at(http::X_FORWARDED_HOST);
	}
	else
	{
		if (!this->_environment.contains(net::meta::SERVER_NAME))
		{
			throw KeyError("'environment' does not contain " + std::string(net::meta::SERVER_NAME));
		}

		raw_host = this->_environment.at(net::meta::SERVER_NAME);
		auto port = this->_get_port(use_x_forwarded_port);
		if (port != (this->is_secure(secure_proxy_ssl_header) ? "443" : "80"))
		{
			raw_host = raw_host + ":" + port;
		}
	}

	return raw_host;
}

std::string Request::_get_port(bool use_x_forwarded_port) const
{
	std::string port;
	if (use_x_forwarded_port && this->_headers.contains(http::X_FORWARDED_PORT))
	{
		port = this->_headers.at(http::X_FORWARDED_PORT);
	}
	else
	{
		if (!this->_environment.contains(net::meta::SERVER_PORT))
		{
			throw KeyError("'environment' does not contain " + std::string(net::meta::SERVER_PORT));
		}

		port = this->_environment.at(net::meta::SERVER_PORT);
	}

	return port;
}

void Request::_parse_form(bool throw_on_invalid_ct)
{
	if (!this->_form.has_value())
	{
		Query post_form;
		if (this->_method == "POST" || this->_method == "PUT" || this->_method == "PATCH")
		{
			post_form = parse_post_form(this, this->_body_reader.get(), throw_on_invalid_ct);
		}

		if (!post_form.empty())
		{
			this->_form = std::move(post_form);
		}

		auto new_query = parse_query(this->_url.raw_query);
		if (!this->_form.has_value())
		{
			this->_form = std::move(new_query);
		}
		else
		{
			for (const auto& item : new_query)
			{
				this->_form->add(item.first, item.second);
			}
		}
	}
}

void Request::_parse_multipart_form()
{
	if (!this->_form.has_value())
	{
		this->_parse_form(false);
	}

	if (this->_multipart_form.has_value())
	{
		return;
	}

	auto reader = this->_multipart_reader(false);
	auto target_form = reader->read_form(this->multipart_max_memory);
	for (const auto& pair : target_form.values)
	{
		this->_form->add(pair.first, pair.second);
	}

	this->_multipart_form = std::move(target_form);
}

void Request::_parse_json_data()
{
	auto [content, ok] = read_body_to_string(
		this, this->_body_reader.get(), mime::APPLICATION_JSON, !this->throw_on_invalid_content_type
	);
	if (ok)
	{
		this->_json = nlohmann::json::parse(content);
	}
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

std::tuple<std::string, bool> read_body_to_string(
	http::Request* request,
	io::ILimitedBufferedReader* body_reader,
	const std::string& target_content_type,
	bool mute_invalid_content_type_error
)
{
	require_non_null(request, "'request' is nullptr", _ERROR_DETAILS_);
	if (!body_reader)
	{
		throw exc::HttpError(400, "missing form body", _ERROR_DETAILS_);
	}

	auto parsed_content_type = parse_content_type(request);
	if (parsed_content_type == target_content_type)
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
			read_full_request_body(buffer, body_reader, content_length);
			return {buffer, true};
		}
	}
	else if (!mute_invalid_content_type_error)
	{
		throw RuntimeError("Content is not '" + target_content_type + "'", _ERROR_DETAILS_);
	}

	return {"", false};
}

Query parse_post_form(
	http::Request* request, io::ILimitedBufferedReader* body_reader, bool mute_invalid_content_type_error
)
{
	Query query;
	auto [content, ok] = read_body_to_string(
		request, body_reader, mime::APPLICATION_X_WWW_FORM_URLENCODED, mute_invalid_content_type_error
	);
	if (ok)
	{
		query = parse_query(content);
	}

	return query;
}

std::string parse_content_type(http::Request* request)
{
	auto content_type = request->get_header(CONTENT_TYPE, "");

	// RFC 7231, section 3.1.1.5 - empty type
	// MAY be treated as application/octet-stream
	if (content_type.empty())
	{
		content_type = mime::APPLICATION_OCTET_STREAM;
	}

	auto media_type_result = mime::parse_media_type(str::string_to_wstring(content_type));

	// Return the first item of the result.
	// In case of some parse error, the first item will be
	// assigned to Content-Type header value.
	return str::wstring_to_string(std::get<0>(media_type_result));
}

__HTTP_END__
