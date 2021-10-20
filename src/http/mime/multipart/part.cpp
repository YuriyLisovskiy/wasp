/**
 * http/mime/multipart/part.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./part.h"

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/net/utility.h>
#include <xalwart.base/path.h>

// Framework libraries.
#include "./body_reader.h"
#include "./quote_printable_reader.h"
#include "./base64_reader.h"
#include "./part_reader.h"
#include "../media_type.h"
#include "../../headers.h"


__HTTP_MIME_MULTIPART_BEGIN__

Part::Part(
	BodyReader* multipart_reader, ssize_t remaining_content_length, bool raw_part,
	ssize_t max_header_length, ssize_t max_headers_count
) :
	remaining_content_length(remaining_content_length),
	multipart_reader(multipart_reader),
	max_header_length(max_header_length),
	max_headers_count(max_headers_count)
{
	if (!this->multipart_reader)
	{
		throw NullPointerException("'multipart_reader' is nullptr", _ERROR_DETAILS_);
	}

	this->populate_headers();
	this->reader = std::make_shared<PartReader>(this);

	// `raw_part` is used to switch between `next_part` and `next_raw_part`.
	if (!raw_part)
	{
		auto transfer_encoding = this->get_header(CONTENT_TRANSFER_ENCODING, "");
		if (str::equal_fold(transfer_encoding, "quoted-printable"))
		{
			this->remove_header(CONTENT_TRANSFER_ENCODING);
			this->reader = std::make_shared<QuotePrintableReader>(this->reader);
		}
		else if (str::equal_fold(transfer_encoding, "base64"))
		{
			this->remove_header(CONTENT_TRANSFER_ENCODING);
			this->reader = std::make_shared<Base64Reader>(this->reader);
		}
	}
}

std::string Part::form_name()
{
	// See https://tools.ietf.org/html/rfc2183 section 2 for EBNF
	// of Content-Disposition value format.
	if (!this->disposition_params.has_value())
	{
		this->parse_content_disposition();
	}

	if (this->disposition != L"form-data")
	{
		return "";
	}

	if (!this->disposition_params->contains(L"name"))
	{
		throw KeyError("disposition does not contain 'name'", _ERROR_DETAILS_);
	}

	return str::wstring_to_string(this->disposition_params->at(L"name"));
}

std::string Part::file_name()
{
	if (!this->disposition_params.has_value())
	{
		this->parse_content_disposition();
	}

	auto filename = this->disposition_params->contains(L"filename") ?
		str::wstring_to_string(this->disposition_params->at(L"filename")) : "";
	if (filename.empty())
	{
		return "";
	}

	// RFC 7578, Section 4.2 requires that if a filename is provided, the
	// directory path information must not be used.
	return path::Path(filename).basename();
}

ssize_t Part::read(std::string& buffer, size_t max_count)
{
	auto bytes_to_read = std::min<ssize_t>(this->remaining_content_length, (ssize_t)max_count);
	ssize_t total_bytes_read = 0;
	std::string bytes;
	while (bytes_to_read > 0)
	{
		auto n_ = this->reader->read(bytes, bytes_to_read);
		if (n_ == 0)
		{
			// EOF
			break;
		}

		total_bytes_read += n_;
		bytes_to_read -= n_;
		buffer += bytes;
		this->remaining_content_length -= n_;
	}

	return total_bytes_read;
}

void Part::parse_content_disposition()
{
	auto content_disposition = str::string_to_wstring(this->get_header(CONTENT_DISPOSITION, ""));
	auto [disposition_string, disposition_map, ok] = mime::parse_media_type(content_disposition);
	this->disposition = disposition_string;
	this->disposition_params = ok ? disposition_map : std::map<std::wstring, std::wstring>{};
}

void Part::populate_headers()
{
	net::parse_headers(
		this->header, this->multipart_reader->buffered_reader.get(), this->max_header_length, this->max_headers_count
	);
}

__HTTP_MIME_MULTIPART_END__
