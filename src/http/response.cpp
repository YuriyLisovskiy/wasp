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
 * An implementation of response.h.
 */

#include "./response.h"


__HTTP_BEGIN__

HttpResponseBase::HttpResponseBase(
	unsigned short int status,
	std::string content_type,
	const std::string& reason,
	const std::string& charset
)
{
	this->_streaming = false;
	this->_headers = collections::Dict<std::string, std::string>(true);
	this->_closed = false;

	if (status != 0)
	{
		if (status < 100 || status > 599)
		{
			throw core::ValueError("HTTP status code must be an integer from 100 to 599.", _ERROR_DETAILS_);
		}

		this->_status = status;
	}
	else
	{
		this->_status = 200;
	}

	this->_charset = charset;

	if (content_type.empty())
	{
		content_type = "text/html; charset=" + this->_charset;
	}

	this->_headers.set("Content-Type", content_type);
}

void HttpResponseBase::set_header(const std::string& key, const std::string& value)
{
	this->_headers.set(key, value);
}

void HttpResponseBase::remove_header(const std::string& key)
{
	this->_headers.remove(key);
}

bool HttpResponseBase::has_header(const std::string& key)
{
	return this->_headers.contains(key);
}

void HttpResponseBase::set_cookie(
	const std::string& name,
	const std::string& value,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool is_secure,
	bool is_http_only
)
{
	this->_cookies.set(name, Cookie(name, value, expires, domain, path, is_secure, is_http_only));
}

void HttpResponseBase::set_signed_cookie(
	const std::string& name,
	const std::string& value,
	const std::string& salt,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool is_secure,
	bool is_http_only
)
{
	// TODO:
}

void HttpResponseBase::delete_cookie(const std::string& name, const std::string& path, const std::string& domain)
{
	bool is_secure = std::strncmp(name.c_str(), "__Secure-", 9) == 0 || std::strncmp(name.c_str(), "__Host-", 7) == 0;
	this->_cookies.set(name, Cookie(name, "", "Thu, 01 Jan 1970 00:00:00 GMT", domain, path, is_secure));
}

std::string HttpResponseBase::get_reason_phrase()
{
	if (!this->_reason_phrase.empty())
	{
		return this->_reason_phrase;
	}

	return internal::HTTP_STATUS.get(this->_status, {"Unknown Status Code", ""}).first;
}

void HttpResponseBase::set_reason_phrase(std::string value)
{
	if (value.empty())
	{
		value = "Unknown Status Code";
	}

	this->_reason_phrase = value;
}

unsigned short int HttpResponseBase::status()
{
	return this->_status;
}

std::string HttpResponseBase::content_type()
{
	return this->_headers.get("Content-Type", "");
}

std::string HttpResponseBase::charset()
{
	return this->_charset;
}

bool HttpResponseBase::is_streaming()
{
	return this->_streaming;
}

void HttpResponseBase::close()
{
	this->_closed = true;
}

void HttpResponseBase::write(const std::string& content)
{
	throw core::HttpError("This HttpResponseBase instance is not writable", _ERROR_DETAILS_);
}

void HttpResponseBase::flush()
{
}

unsigned long int HttpResponseBase::tell()
{
	throw core::HttpError("This HttpResponseBase instance cannot tell its position", _ERROR_DETAILS_);
}

bool HttpResponseBase::readable()
{
	return false;
}

bool HttpResponseBase::seekable()
{
	return false;
}

bool HttpResponseBase::writable()
{
	return false;
}

void HttpResponseBase::write_lines(const std::vector<std::string>& lines)
{
	throw core::HttpError("This HttpResponseBase instance is not writable", _ERROR_DETAILS_);
}

std::string HttpResponseBase::serialize_headers()
{
	auto expr = [](const std::pair<std::string, std::string>& _p) -> std::string { return _p.first + ": " + _p.second; };
	std::string result;
	for (auto it = this->_headers.cbegin(); it != this->_headers.cend(); it++)
	{
		result.append(expr(*it));
		if (std::next(it) != this->_headers.cend())
		{
			result.append("\r\n");
		}
	}

	return result;
}


// HttpResponse implementation
HttpResponse::HttpResponse(
	const std::string& content,
	unsigned short status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : HttpResponseBase(status, content_type, reason, charset)
{
	this->_content = content;
	this->_streaming = false;
}

void HttpResponse::set_content(const std::string& content)
{
	this->_content = content;
}

void HttpResponse::write(const std::string& content)
{
	this->_content.append(content);
}

unsigned long int HttpResponse::tell()
{
	return this->_content.size();
}

bool HttpResponse::writable()
{
	return true;
}

void HttpResponse::write_lines(const std::vector<std::string>& lines)
{
	for (const auto & line : lines)
	{
		this->write(line);
	}
}

std::string HttpResponse::serialize()
{
	this->set_header("Date", dt::gmtnow().strftime("%a, %d %b %Y %T %Z"));
	this->set_header("Content-Length", std::to_string(this->_content.size()));

	auto reason_phrase = this->get_reason_phrase();

	auto headers = this->serialize_headers();

	return "HTTP/1.1 " + std::to_string(this->_status) + " " + reason_phrase + "\r\n" +
		   headers + "\r\n\r\n" + this->_content;
}


// StreamingHttpResponse implementation
StreamingHttpResponse::StreamingHttpResponse(
	unsigned short int status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : HttpResponseBase(status, content_type, reason, charset)
{
	this->_streaming = true;
}

std::string StreamingHttpResponse::serialize()
{
	throw core::HttpError("This StreamingHttpResponse or its child instance cannot be serialized", _ERROR_DETAILS_);
}


// FileResponse implementation
FileResponse::FileResponse(
	std::string file_path,
	bool asAttachment,
	unsigned short status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : StreamingHttpResponse(status, content_type, reason, charset),
    _bytes_read(0),
    _total_bytes_read(0),
	_as_attachment(asAttachment),
	_file_path(std::move(file_path)),
	_headers_is_got(false)
{
	if (!path::exists(this->_file_path))
	{
		throw core::FileDoesNotExistError("file '" + this->_file_path + "' does not exist", _ERROR_DETAILS_);
	}

	// Initializing file stream.
	this->_file_stream = std::ifstream(this->_file_path, std::ifstream::binary | std::ios::ate);
	this->_file_size = this->_file_stream.tellg();
	this->_file_stream.seekg(0);
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
	auto encoding_map = collections::Dict<std::string, std::string>({
		{"bzip2", "application/x-bzip"},
		{"gzip", "application/gzip"},
		{"xz", "application/x-xz"}
	});
	this->set_header("Content-Length", std::to_string(path::get_size(this->_file_path)));
	if (str::starts_with(this->_headers.get("Content-Type", ""), "text/html"))
	{
		std::string content_type, encoding;
		mime::guess_content_type(this->_file_path, content_type, encoding);
		content_type = encoding_map.get(encoding, content_type);
		this->_headers.set("Content-Type", !content_type.empty() ? content_type : "application/octet-stream");
	}

	std::string disposition = this->_as_attachment ? "attachment" : "inline";
	std::string file_expr;
	std::string file_name = path::basename(this->_file_path);
	try
	{
		file_expr = "filename=\"" + encoding::encode(file_name, encoding::ASCII) + "\"";
	}
	catch (const core::EncodingError& e)
	{
		file_expr = "filename*=utf-8''" + encoding::quote(file_name);
	}

	this->_headers.set("Content-Disposition", disposition + "; " + file_expr);
}

std::string FileResponse::_get_headers_chunk()
{
	auto reason_phrase = this->get_reason_phrase();
	this->_set_headers();
	this->set_header("Date", dt::gmtnow().strftime("%a, %d %b %Y %T %Z"));
	auto headers = this->serialize_headers();

	std::string headers_chunk = "HTTP/1.1 " + std::to_string(this->_status) + " " + reason_phrase + "\r\n"
		+ headers + "\r\n\r\n";
	this->_bytes_read = headers_chunk.size();

	return headers_chunk;
}

unsigned long int FileResponse::tell()
{
	return this->_bytes_read;
}

void FileResponse::close()
{
	StreamingHttpResponse::close();
	this->_file_stream.close();
}


// HttpResponseRedirectBase implementation
HttpResponseRedirectBase::HttpResponseRedirectBase(
	const std::string& redirect_to,
	const std::string& content,
	unsigned short int status,
	const std::string& content_type,
	const std::string& reason,
	const std::string& charset
) : HttpResponse(content, status, content_type, reason, charset)
{
	if (status < 300 || status > 399)
	{
		throw core::ValueError("invalid status", _ERROR_DETAILS_);
	}

	this->set_header("Location", encoding::encode_url(redirect_to));
	Url url(redirect_to);
	if (!url.scheme().empty() && this->_allowed_hosts.find(url.scheme()) == this->_allowed_hosts.end())
	{
		throw core::DisallowedRedirect("Unsafe redirect to URL with protocol " + url.scheme(), _ERROR_DETAILS_);
	}
}

std::string HttpResponseRedirectBase::url()
{
	return this->_headers.get("Location");
}


// HttpResponseRedirect implementation
HttpResponseRedirect::HttpResponseRedirect(
	const std::string& redirect_to,
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponseRedirectBase(redirect_to, content, 302, content_type, "", charset)
{
}


// HttpResponsePermanentRedirect implementation
HttpResponsePermanentRedirect::HttpResponsePermanentRedirect(
	const std::string& redirect_to,
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponseRedirectBase(redirect_to, content, 301, content_type, "", charset)
{
}


// HttpResponseNotModified implementation
HttpResponseNotModified::HttpResponseNotModified(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 304, content_type, "", charset)
{
	this->remove_header("Content-Type");
}

void HttpResponseNotModified::set_content(const std::string& content)
{
	if (!content.empty())
	{
		throw core::AttributeError("You cannot set content to a 304 (Not Modified) response", _ERROR_DETAILS_);
	}

	this->_content = "";
}


// HttpResponseBadRequest implementation
HttpResponseBadRequest::HttpResponseBadRequest(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 400, content_type, "", charset)
{
}


// HttpResponseNotFound implementation
HttpResponseNotFound::HttpResponseNotFound(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 404, content_type, "", charset)
{
}


// HttpResponseForbidden implementation
HttpResponseForbidden::HttpResponseForbidden(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 403, content_type, "", charset)
{
}


// HttpResponseForbidden implementation
HttpResponseNotAllowed::HttpResponseNotAllowed(
	const std::string& content,
	const std::vector<std::string>& permitted_methods,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 405, content_type, "", charset)
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


// HttpResponseGone implementation
HttpResponseGone::HttpResponseGone(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 410, content_type, "", charset)
{
}


// HttpResponseEntityTooLarge implementation
HttpResponseEntityTooLarge::HttpResponseEntityTooLarge(
		const std::string& content,
		const std::string& content_type,
		const std::string& charset
) : HttpResponse(content, 413, content_type, "", charset)
{
}


// HttpResponseServerError implementation
HttpResponseServerError::HttpResponseServerError(
	const std::string& content,
	const std::string& content_type,
	const std::string& charset
) : HttpResponse(content, 500, content_type, "", charset)
{
}

__HTTP_END__
