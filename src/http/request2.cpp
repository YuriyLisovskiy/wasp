/**
 * http/request.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./request2.h"

// Base libraries.
#include <xalwart.base/string_utils.h>
#include <xalwart.base/net/request_context.h>


__HTTP_BEGIN__

std::pair<std::unique_ptr<multipart::Reader>, ParseFormError> Request2::multipart_reader(bool allow_mixed) const
{
	auto content_type = this->header.get("Content-Type", "");
	if (content_type.empty())
	{
		return {nullptr, ParseFormError::NotMultipart};
	}

	auto [media_type, params, err] = parse_media_type(content_type);
	if (
		err != ParseMediaTypeError::Nil ||
		!(media_type == "multipart/form-data" || allow_mixed && media_type == "multipart/mixed")
	)
	{
		return {nullptr, ParseFormError::NotMultipart};
	}

	if (!params.contains("boundary"))
	{
		return {nullptr, ParseFormError::MissingBoundary};
	}

	return {std::make_unique<multipart::Reader>(this->body, params.at("boundary")), ParseFormError::Nil};
}

ParseFormError Request2::parse_form()
{
	ParseFormError err = ParseFormError::Nil;
	if (!this->post_form)
	{
		if (this->method == "POST" || this->method == "PUT" || this->method == "PATCH")
		{
			auto [pf, error] = parse_post_form(this);
			this->post_form = std::make_unique<Query>(pf), err = error;
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
		for (auto it = new_query->begin(); it != new_query->end(); it++)
		{
			this->form->add(it->first, it->second);
		}
	}

	return err;
}

ParseFormError Request2::parse_multipart_form(long long int max_memory)
{
	if (!this->form)
	{
		auto err = this->parse_form();
		if (err != ParseFormError::Nil)
		{
			return err;
		}
	}

	if (this->multipart_form)
	{
		return ParseFormError::Nil;
	}

	auto [reader, r_err] = this->multipart_reader(false);
	if (r_err != ParseFormError::Nil)
	{
		return r_err;
	}

	auto [f, f_err] = reader->read_form(max_memory);
	if (f_err != ParseFormError::Nil)
	{
		return f_err;
	}

	if (!this->post_form)
	{
		this->post_form = std::make_unique<Query>();
	}

	for (const auto& pair : f->values)
	{
		this->form->add(pair.first, pair.second);

		// 'post_form' should also be populated.
		this->post_form->add(pair.first, pair.second);
	}

	this->multipart_form = std::move(f);
	return ParseFormError::Nil;
}

std::tuple<std::string, std::map<std::string, std::string>, ParseMediaTypeError> parse_media_type(
	const std::string& content_type
)
{
	// TODO:
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

std::tuple<std::unique_ptr<Request2>, std::unique_ptr<BaseException>> new_request(
	const std::string& method, const std::string& raw_url, const std::string& proto,
	int proto_major, int proto_minor, const collections::MultiDictionary<std::string, std::string>& header,
	io::IReader* body_reader, long long int content_length
)
{
	if (!valid_method(method))
	{
		return {nullptr, std::make_unique<ArgumentError>("invalid method: '" + method + "'", _ERROR_DETAILS_)};
	}

	auto url = parse_url(raw_url);
	url.host = remove_empty_port(url.host);
	return {std::make_unique<Request2>(
		method, url, proto, proto_major, proto_minor, header, body_reader, content_length
	), nullptr};
}

std::tuple<Query, ParseFormError> parse_post_form(Request2* request)
{
	if (!request->body)
	{
		return {{}, ParseFormError::MissingFormBody};
	}

	auto content_type = request->header.get("Content-Type", "");

	// RFC 7231, section 3.1.1.5 - empty type
	//   MAY be treated as application/octet-stream
	if (content_type.empty())
	{
		content_type = "application/octet-stream";
	}

	Query query;
	auto [ct, _, err] = parse_media_type(content_type);
	if (ct == "application/x-www-form-urlencoded")
	{
		long long int max_form_size = (long long int)1 << (63 - 1);
		std::string buffer;
		auto e = request->body->read(buffer, max_form_size + 1);
		switch (e)
		{
			case net::SocketReaderState::Done:
				break;
			case net::SocketReaderState::ConnectionBroken:
			case net::SocketReaderState::Failed:
			case net::SocketReaderState::Eof:
				return {{}, ParseFormError::ReadingFailure};
			case net::SocketReaderState::TimedOut:
				return {{}, ParseFormError::ReadingTimedOut};
			default:
				break;
		}

		if (buffer.size() > max_form_size)
		{
			return {{}, ParseFormError::PostTooLarge};
		}

		query = parse_query(buffer);
	}
	else if (ct == "multipart/form-data")
	{
		// handled by 'parse_multipart_form' (which is calling us, or should be).
	}

	return {query, ParseFormError::Nil};
}

__HTTP_END__
