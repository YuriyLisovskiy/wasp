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
#include "./mime/media_type.h"


__HTTP_BEGIN__

AbstractResponse::AbstractResponse(
	unsigned short int status, std::string content_type, std::string reason, std::string charset
) : streaming(false), headers({}), closed(false), reason_phrase(std::move(reason)), charset(std::move(charset))
{
	if (status != 0)
	{
		if (status < 100 || status > 599)
		{
			throw ValueError("HTTP status code must be an integer from 100 to 599.", _ERROR_DETAILS_);
		}

		this->status = status;
	}
	else
	{
		this->status = 200;
	}

	if (content_type.empty())
	{
		content_type = "text/html; charset=" + this->charset;
	}

	this->set_header(CONTENT_TYPE, content_type);
}

void AbstractResponse::set_cookie(const Cookie& cookie)
{
	if (!cookie.same_site().empty())
	{
		auto same_site_lower = str::to_lower(cookie.same_site());
		if (same_site_lower != "lax" && same_site_lower != "strict")
		{
			throw ValueError(R"(samesite must be "lax" or "strict".)", _ERROR_DETAILS_);
		}
	}

	if (this->cookies.contains(cookie.name()))
	{
		this->cookies[cookie.name()] = cookie;
	}
	else
	{
		this->cookies.insert(std::make_pair(cookie.name(), cookie));
	}
}

void AbstractResponse::set_signed_cookie(const std::string& secret_key, const std::string& salt, const Cookie& cookie)
{
	auto signed_value = get_cookie_signer(secret_key, cookie.name() + salt).sign(cookie.value());
	this->set_cookie(Cookie(
		cookie.name(), signed_value, cookie.max_age(), cookie.domain(),
		cookie.path(), cookie.is_secure(), cookie.is_http_only(), cookie.same_site()
	));
}

void AbstractResponse::delete_cookie(const std::string& name, const std::string& path, const std::string& domain)
{
	bool is_secure = name.starts_with("__Secure-") || name.starts_with("__Host-");
	auto cookie = Cookie(name, "", 0, domain, path, is_secure);
	if (this->cookies.contains(name))
	{
		this->cookies[name] = cookie;
	}
	else
	{
		this->cookies.insert(std::make_pair(name, cookie));
	}
}

std::string AbstractResponse::get_reason_phrase() const
{
	if (!this->reason_phrase.empty())
	{
		return this->reason_phrase;
	}

	return net::get_status_by_code(this->status).first.phrase;
}

std::string AbstractResponse::serialize_headers() const
{
	auto expr = [](const std::pair<std::string, std::string>& _p) -> std::string
	{
		return _p.first + ": " + _p.second;
	};

	std::string result;
	for (auto it = this->headers.begin(); it != this->headers.end(); it++)
	{
		result.append(expr(*it));
		if (std::next(it) != this->headers.end())
		{
			result.append("\r\n");
		}
	}

	for (auto it = this->cookies.begin(); it != this->cookies.end(); it++)
	{
		result.append(it->second.to_string());
		if (std::next(it) != this->cookies.end())
		{
			result.append("\r\n");
		}
	}

	return result;
}

std::string BaseResponse::serialize()
{
	auto content = this->get_content();
	this->set_header(DATE, dt::Datetime::utc_now().strftime("%a, %d %b %Y %T GMT"));
	this->set_header(CONTENT_LENGTH, std::to_string(content.size()));
	auto reason_phrase = this->get_reason_phrase();
	auto headers = this->serialize_headers();
	return "HTTP/1.1 " + std::to_string(this->status) + " " + reason_phrase + "\r\n" +
	   headers + "\r\n\r\n" + content;
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
	auto encoding_map = std::map<std::string, std::string>({
		{"bzip2", "application/x-bzip"},
		{"gzip", "application/gzip"},
		{"xz", "application/x-xz"}
	});
	this->set_header(CONTENT_LENGTH, std::to_string(path::get_size(this->_file_path)));
	if (this->get_header(CONTENT_TYPE, "").starts_with("text/html"))
	{
		std::string content_type, encoding;
		mime::guess_content_type(this->_file_path, content_type, encoding);
		if (encoding_map.contains(encoding))
		{
			content_type = encoding_map.at(encoding);
		}

		this->set_header(CONTENT_TYPE, !content_type.empty() ? content_type : "application/octet-stream");
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

	this->set_header(CONTENT_DISPOSITION, disposition + "; " + file_expr);
}

std::string FileResponse::_get_headers_chunk()
{
	auto reason_phrase = this->get_reason_phrase();
	this->_set_headers();
	this->set_header("Date", dt::Datetime::utc_now().strftime("%a, %d %b %Y %T GMT"));
	auto headers = this->serialize_headers();
	std::string headers_chunk = "HTTP/1.1 " + std::to_string(this->status) + " " + reason_phrase + "\r\n"
		+ headers + "\r\n\r\n";
	this->_bytes_read = headers_chunk.size();
	return headers_chunk;
}

RedirectBase::RedirectBase(
	const std::string& redirect_to,
	unsigned short int status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : Response("", status, content_type, reason, charset)
{
	if (status < 300 || status > 399)
	{
		throw ArgumentError("invalid status: " + std::to_string(status), _ERROR_DETAILS_);
	}

	this->set_header(LOCATION, http::parse_url(redirect_to).str());
	auto url = http::parse_url(redirect_to);
	if (!url.scheme.empty() && ALLOWED_SCHEMES.find(url.scheme) == ALLOWED_SCHEMES.end())
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

	this->content = "";
}

NotAllowed::NotAllowed(
	const std::string& content,
	const std::vector<std::string>& permitted_methods,
	const std::string& content_type,
	const std::string& charset
) : Response(content, 405, content_type, "", charset)
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

	this->set_header(ALLOW, methods);
}

__HTTP_END__
