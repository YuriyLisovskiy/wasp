/**
 * http/response.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./response.h"

// Base libraries.
#include <xalwart.base/path.h>
#include <xalwart.base/string_utils.h>
#include <xalwart.base/encoding.h>
#include <xalwart.base/net/status.h>

// Framework libraries.
#include "./url.h"
#include "./utility.h"
#include "./exceptions.h"
#include "../core/mime_types.h"


__HTTP_BEGIN__

ResponseBase::ResponseBase(
	unsigned short int status,
	std::string content_type,
	const std::string& reason,
	const std::string& charset
)
{
	this->_streaming = false;
	this->_headers = collections::Dictionary<std::string, std::string>();
	this->_closed = false;

	if (status != 0)
	{
		if (status < 100 || status > 599)
		{
			throw ValueError("HTTP status code must be an integer from 100 to 599.", _ERROR_DETAILS_);
		}

		this->_status = status;
	}
	else
	{
		this->_status = 200;
	}

	this->_reason_phrase = reason;
	this->_charset = charset;

	if (content_type.empty())
	{
		content_type = "text/html; charset=" + this->_charset;
	}

	this->_headers.set("Content-Type", content_type);
}

void ResponseBase::set_cookie(
	const std::string& name,
	const std::string& value,
	long max_age,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool is_secure,
	bool is_http_only,
	const std::string& same_site
)
{
	std::string same_site_;
	if (!same_site.empty())
	{
		auto ss_lower = str::lower(same_site_);
		if (ss_lower != "lax" && ss_lower != "strict")
		{
			throw ValueError(R"(samesite must be "lax" or "strict".)", _ERROR_DETAILS_);
		}

		same_site_ = same_site;
	}

	auto cookie = Cookie(
		name, value, max_age, expires, domain, path, is_secure, is_http_only, same_site_
	);
	this->_cookies.set(name, cookie);
}

void ResponseBase::set_signed_cookie(
	const std::string& secret_key,
	const std::string& name,
	const std::string& value,
	const std::string& salt,
	long max_age,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool is_secure,
	bool is_http_only,
	const std::string& same_site
)
{
	auto signed_value = get_cookie_signer(secret_key, name + salt).sign(value);
	this->set_cookie(name, signed_value, max_age, expires, domain, path, is_secure, is_http_only, same_site);
}

void ResponseBase::delete_cookie(
	const std::string& name, const std::string& path, const std::string& domain
)
{
	bool is_secure = std::strncmp(name.c_str(), "__Secure-", 9) == 0 || std::strncmp(name.c_str(), "__Host-", 7) == 0;
	this->_cookies.set(name, Cookie(name, "", 0, "Thu, 01 Jan 1970 00:00:00 GMT", domain, path, is_secure));
}

std::string ResponseBase::get_reason_phrase()
{
	if (!this->_reason_phrase.empty())
	{
		return this->_reason_phrase;
	}

	return net::get_status_by_code(this->_status).first.phrase;
}

void ResponseBase::set_reason_phrase(std::string value)
{
	if (value.empty())
	{
		value = "Unknown Status";
	}

	this->_reason_phrase = value;
}

std::string ResponseBase::serialize_headers()
{
	auto expr = [](const std::pair<std::string, std::string>& _p) -> std::string
	{
		return _p.first + ": " + _p.second;
	};

	std::string result;
	for (auto it = this->_headers.begin(); it != this->_headers.end(); it++)
	{
		result.append(expr(*it));
		if (std::next(it) != this->_headers.end())
		{
			result.append("\r\n");
		}
	}

	for (auto it = this->_cookies.begin(); it != this->_cookies.end(); it++)
	{
		result.append(it->second.to_string());
		if (std::next(it) != this->_cookies.end())
		{
			result.append("\r\n");
		}
	}

	return result;
}

void Response::write_lines(const std::vector<std::string>& lines)
{
	for (const auto & line : lines)
	{
		this->write(line);
	}
}

std::string Response::serialize()
{
	this->set_header("Date", dt::Datetime::utc_now().strftime("%a, %d %b %Y %T GMT"));
	this->set_header("Content-Length", std::to_string(this->_content.size()));
	auto reason_phrase = this->get_reason_phrase();
	auto headers = this->serialize_headers();
	return "HTTP/1.1 " + std::to_string(this->_status) + " " + reason_phrase + "\r\n" +
	   headers + "\r\n\r\n" + this->_content;
}

FileResponse::FileResponse(
	std::string file_path,
	bool as_attachment,
	unsigned short status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : StreamingResponse(status, content_type, reason, charset),
    _bytes_read(0),
    _total_bytes_read(0),
    _as_attachment(as_attachment),
	_file_path(std::move(file_path)),
	_headers_is_got(false)
{
	if (!path::exists(this->_file_path))
	{
		throw exc::FileDoesNotExist("file '" + this->_file_path + "' does not exist", _ERROR_DETAILS_);
	}
	else
	{
		// Initializing file stream.
		this->_file_stream = std::ifstream(this->_file_path, std::ifstream::binary | std::ios::ate);
		this->_file_size = this->_file_stream.tellg();
		this->_file_stream.seekg(0);
	}
}

std::string FileResponse::get_chunk()
{
	std::string chunk;
	if (!this->_headers_is_got)
	{
		chunk = this->_get_headers_chunk();
		this->_headers_is_got = true;
	}
	else
	{
		size_t bytes_to_read;
		if ((this->_total_bytes_read + FileResponse::CHUNK_SIZE) > this->_file_size)
		{
			bytes_to_read = this->_file_size - this->_total_bytes_read;
		}
		else
		{
			bytes_to_read = FileResponse::CHUNK_SIZE;
		}

		if (bytes_to_read > 0)
		{
			chunk = std::string(FileResponse::CHUNK_SIZE, '\0');

			this->_file_stream.read(chunk.data(), bytes_to_read);
			this->_bytes_read = this->_file_stream.gcount();
			this->_total_bytes_read += this->_bytes_read;
		}
	}

	return chunk.substr(0, this->_bytes_read);
}

void FileResponse::_set_headers()
{
	auto encoding_map = collections::Dictionary<std::string, std::string>({
		{"bzip2", "application/x-bzip"},
		{"gzip", "application/gzip"},
		{"xz", "application/x-xz"}
	});
	this->set_header("Content-Length", std::to_string(path::get_size(this->_file_path)));
	if (this->_headers.get("Content-Type", "").starts_with("text/html"))
	{
		std::string content_type, encoding;
		core::mime::guess_content_type(this->_file_path, content_type, encoding);
		content_type = encoding_map.get(encoding, content_type);
		this->_headers.set("Content-Type", !content_type.empty() ? content_type : "application/octet-stream");
	}

	std::string disposition = this->_as_attachment ? "attachment" : "inline";
	std::string file_expr;
	std::string file_name = path::basename(this->_file_path);
	try
	{
		file_expr = "filename=\"" + encoding::encode(file_name, encoding::Encoding::ASCII) + "\"";
	}
	catch (const EncodingError& e)
	{
		file_expr = "filename*=utf-8''" + encoding::quote(file_name);
	}

	this->_headers.set("Content-Disposition", disposition + "; " + file_expr);
}

std::string FileResponse::_get_headers_chunk()
{
	auto reason_phrase = this->get_reason_phrase();
	this->_set_headers();
	this->set_header("Date", dt::Datetime::utc_now().strftime("%a, %d %b %Y %T GMT"));
	auto headers = this->serialize_headers();
	std::string headers_chunk = "HTTP/1.1 " + std::to_string(this->_status) + " " + reason_phrase + "\r\n"
		+ headers + "\r\n\r\n";
	this->_bytes_read = headers_chunk.size();
	return headers_chunk;
}

__HTTP_END__


__HTTP_RESP_BEGIN__

RedirectBase::RedirectBase(
	const std::string& redirect_to,
	unsigned short int status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : Response(status, "", content_type, reason, charset)
{
	if (status < 300 || status > 399)
	{
		throw ArgumentError("invalid status: " + std::to_string(status), _ERROR_DETAILS_);
	}

	this->set_header("Location", http::parse_url(redirect_to).str());
	auto url = http::parse_url(redirect_to);
	if (!url.scheme.empty() && this->_allowed_schemes.find(url.scheme) == this->_allowed_schemes.end())
	{
		throw exc::DisallowedRedirect("unsafe redirect to URL with protocol " + url.scheme, _ERROR_DETAILS_);
	}
}

void NotModified::set_content(const std::string& content)
{
	if (!content.empty())
	{
		throw ArgumentError("You cannot set content to a 304 (Not Modified) response.", _ERROR_DETAILS_);
	}

	this->_content = "";
}

NotAllowed::NotAllowed(
	const std::string& content,
	const std::vector<std::string>& permitted_methods,
	const std::string& content_type,
	const std::string& charset
) : Response(405, content, content_type, "", charset)
{
	std::string methods;
	for (size_t i = 0; i < permitted_methods.size(); i++)
	{
		methods += permitted_methods[i];
		if (i < permitted_methods.size() - 1)
		{
			methods += ", ";
		}
	}

	this->set_header("Allow", methods);
}

__HTTP_RESP_END__
