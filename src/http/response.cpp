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

/*
 * HTTP/1.1 response base implementation.
 * TODO: write docs
 */

#include "response.h"


__WASP_BEGIN__

HttpResponseBase::HttpResponseBase(
	unsigned short int status,
	std::string contentType,
	const std::string& reason,
	const std::string& charset
)
{
	this->_streaming = false;
	this->_headers = Dict<std::string, std::string>(true);
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

	this->_charset = charset;

	if (contentType.empty())
	{
		contentType = "text/html; charset=" + this->_charset;
	}
	this->_headers.set("Content-Type", contentType);
}

void HttpResponseBase::setHeader(const std::string& key, const std::string& value)
{
	this->_headers.set(key, value);
}

void HttpResponseBase::removeHeader(const std::string& key)
{
	this->_headers.remove(key);
}

bool HttpResponseBase::hasHeader(const std::string& key)
{
	return this->_headers.contains(key);
}

void HttpResponseBase::setCookie(
	const std::string& name,
	const std::string& value,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool isSecure,
	bool isHttpOnly
)
{
	this->_cookies.set(name, Cookie(name, value, expires, domain, path, isSecure, isHttpOnly));
}

void HttpResponseBase::setSignedCookie(
	const std::string& name,
	const std::string& value,
	const std::string& salt,
	const std::string& expires,
	const std::string& domain,
	const std::string& path,
	bool isSecure,
	bool isHttpOnly
)
{
	// TODO:
}

void HttpResponseBase::deleteCookie(const std::string& name, const std::string& path, const std::string& domain)
{
	bool isSecure = std::strncmp(name.c_str(), "__Secure-", 9) == 0 || std::strncmp(name.c_str(), "__Host-", 7) == 0;
	this->_cookies.set(name, Cookie(name, "", "Thu, 01 Jan 1970 00:00:00 GMT", domain, path, isSecure));
}

std::string HttpResponseBase::getReasonPhrase()
{
	if (!this->_reasonPhrase.empty())
	{
		return this->_reasonPhrase;
	}
	return internal::HTTP_STATUS.get(this->_status, {"Unknown Status Code", ""}).first;
}

void HttpResponseBase::setReasonPhrase(std::string value)
{
	if (value.empty())
	{
		value = "Unknown Status Code";
	}
	this->_reasonPhrase = value;
}

void HttpResponseBase::close()
{
	this->_closed = true;
}

void HttpResponseBase::write(const std::string& content)
{
	throw HttpError("This HttpResponseBase instance is not writable", _ERROR_DETAILS_);
}

void HttpResponseBase::flush()
{
}

unsigned long int HttpResponseBase::tell()
{
	throw HttpError("This HttpResponseBase instance cannot tell its position", _ERROR_DETAILS_);
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

void HttpResponseBase::writeLines(const std::vector<std::string>& lines)
{
	throw HttpError("This HttpResponseBase instance is not writable", _ERROR_DETAILS_);
}

std::string HttpResponseBase::serializeHeaders()
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
	const std::string& contentType,
	const std::string& reason,
	const std::string& charset
) : HttpResponseBase(status, contentType, reason, charset)
{
	this->_content = content;
	this->_streaming = false;
}

void HttpResponse::setContent(const std::string& content)
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

void HttpResponse::writeLines(const std::vector<std::string>& lines)
{
	for (const auto & line : lines)
	{
		this->write(line);
	}
}

std::string HttpResponse::serialize()
{
	this->setHeader("Date", dt::gmtnow().strftime("%a, %d %b %Y %T %Z"));
	this->setHeader("Content-Length", std::to_string(this->_content.size()));

	auto reasonPhrase = this->getReasonPhrase();

	auto headers = this->serializeHeaders();

	return "HTTP/1.1 " + std::to_string(this->_status) + " " + reasonPhrase + "\r\n" +
		   headers + "\r\n\r\n" + this->_content;
}


// FileResponse implementation
FileResponse::FileResponse(
	bool asAttachment,
	const std::string& filePath,
	unsigned short status,
	const std::string& contentType,
	const std::string& reason,
	const std::string& charset
) : HttpResponseBase(status, contentType, reason, charset)
{
	this->_asAttachment = asAttachment;
	this->_filePath = filePath;
	if (!path::exists(this->_filePath))
	{
		throw FileDoesNotExistError("file '" + this->_filePath + "' does not exist", _ERROR_DETAILS_);
	}
	this->_read();
}

std::string FileResponse::serialize()
{
	auto reasonPhrase = this->getReasonPhrase();
	this->_setHeaders();
	this->setHeader("Date", dt::gmtnow().strftime("%a, %d %b %Y %T %Z"));
	auto headers = this->serializeHeaders();
	return "HTTP/1.1 " + std::to_string(this->_status) + " " + reasonPhrase + "\r\n" +
		   headers + "\r\n\r\n" + std::string(this->_content.begin(), this->_content.end());
}

void FileResponse::_read()
{
	std::ifstream is(this->_filePath);
	std::istream_iterator<char> start(is), end;
	this->_content = std::vector<char>(start, end);
}

void FileResponse::_setHeaders()
{
	auto encodingMap = Dict<std::string, std::string>({
		{"bzip2", "application/x-bzip"},
		{"gzip", "application/gzip"},
		{"xz", "application/x-xz"}
	});
	this->setHeader("Content-Length", std::to_string(this->_content.size()));
	if (str::startsWith(this->_headers.get("Content-Type", ""), "text/html"))
	{
		std::string contentType, encoding;
		mime::guessContentType(this->_filePath, contentType, encoding);
		contentType = encodingMap.get(encoding, contentType);
		this->_headers.set("Content-Type", !contentType.empty() ? contentType : "application/octet-stream");
	}
	std::string disposition = this->_asAttachment ? "attachment" : "inline";
	std::string fileExpr;
	std::string fileName = path::baseName(this->_filePath);
	try
	{
		fileExpr = "filename=\"" + encoding::encode(fileName, encoding::ASCII) + "\"";
	}
	catch (const EncodingError& e)
	{
		fileExpr = "filename*=utf-8''" + encoding::quote(fileName);
	}
	this->_headers.set("Content-Disposition", disposition + "; " + fileExpr);
}


// HttpResponseRedirectBase implementation
HttpResponseRedirectBase::HttpResponseRedirectBase(
	const std::string& redirectTo,
	const std::string& content,
	unsigned short int status,
	const std::string& contentType,
	const std::string& reason,
	const std::string& charset
) : HttpResponse(content, status, contentType, reason, charset)
{
	if (status < 300 || status > 399)
	{
		throw ValueError("invalid status", _ERROR_DETAILS_);
	}
	this->setHeader("Location", encoding::encodeUrl(redirectTo));
	internal::UrlParser parser;
	parser.parse(redirectTo);
	if (!parser.scheme().empty() && this->_allowedHosts.find(parser.scheme()) == this->_allowedHosts.end())
	{
		throw DisallowedRedirect("Unsafe redirect to URL with protocol " + parser.scheme(), _ERROR_DETAILS_);
	}
}

std::string HttpResponseRedirectBase::url()
{
	return this->_headers.get("Location");
}


// HttpResponseRedirect implementation
HttpResponseRedirect::HttpResponseRedirect(
	const std::string& redirectTo,
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponseRedirectBase(redirectTo, content, 302, contentType, "", charset)
{
}


// HttpResponsePermanentRedirect implementation
HttpResponsePermanentRedirect::HttpResponsePermanentRedirect(
	const std::string& redirectTo,
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponseRedirectBase(redirectTo, content, 301, contentType, "", charset)
{
}


// HttpResponseNotModified implementation
HttpResponseNotModified::HttpResponseNotModified(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 304, contentType, "", charset)
{
	this->removeHeader("Content-Type");
}

void HttpResponseNotModified::setContent(const std::string& content)
{
	if (!content.empty())
	{
		throw AttributeError("You cannot set content to a 304 (Not Modified) response", _ERROR_DETAILS_);
	}
	this->_content = "";
}


// HttpResponseBadRequest implementation
HttpResponseBadRequest::HttpResponseBadRequest(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 400, contentType, "", charset)
{
}


// HttpResponseNotFound implementation
HttpResponseNotFound::HttpResponseNotFound(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 404, contentType, "", charset)
{
}


// HttpResponseForbidden implementation
HttpResponseForbidden::HttpResponseForbidden(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 403, contentType, "", charset)
{
}


// HttpResponseForbidden implementation
HttpResponseNotAllowed::HttpResponseNotAllowed(
	const std::string& content,
	const std::vector<std::string>& permittedMethods,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 405, contentType, "", charset)
{
	std::string methods;
	for (size_t i = 0; i < permittedMethods.size(); i++)
	{
		methods += permittedMethods[i];
		if (i < permittedMethods.size() - 1)
		{
			methods += ", ";
		}
	}
	this->setHeader("Allow", methods);
}


// HttpResponseGone implementation
HttpResponseGone::HttpResponseGone(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 410, contentType, "", charset)
{
}


// HttpResponseServerError implementation
HttpResponseServerError::HttpResponseServerError(
	const std::string& content,
	const std::string& contentType,
	const std::string& charset
) : HttpResponse(content, 500, contentType, "", charset)
{
}

__WASP_END__
