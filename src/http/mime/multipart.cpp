/**
 * http/mime/multipart.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./multipart.h"

// C++ libraries.
#include <filesystem>

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/net/utility.h>

// Framework libraries.
#include "./quote_printable_reader.h"
#include "../exceptions.h"
#include "../headers.h"
#include "../../core/media_type.h"


__HTTP_MULTIPART_BEGIN__

files::UploadedFile FileHeader::open() const
{
	if (this->content.empty())
	{
		return files::UploadedFile::from_path(this->tmp_file);
	}

	return files::UploadedFile::from_content(this->content);
}

void Form::remove_all() const
{
	for (const auto& fhs : this->files)
	{
		for (const auto& fh : fhs.second)
		{
			if (fh && !fh->tmp_file.empty())
			{
				std::filesystem::remove(fh->tmp_file);
			}
		}
	}
}

Part::Part(Reader* multipart_reader, ssize_t content_length, bool raw_part, std::string remaining_bytes) :
	content_length(content_length), multipart_reader(multipart_reader), remaining_bytes(std::move(remaining_bytes))
{
	if (!this->multipart_reader)
	{
		throw NullPointerException("'multipart_reader' is nullptr", _ERROR_DETAILS_);
	}

	this->populate_headers();
	this->reader = std::make_shared<PartReader>(this, this->remaining_bytes);

	// `raw_part` is used to switch between `next_part` and `next_raw_part`.
	if (!raw_part)
	{
		if (str::equal_fold(
			str::string_to_wstring(this->get_header(CONTENT_TRANSFER_ENCODING, "")), L"quoted-printable"
		))
		{
			this->remove_header(CONTENT_TRANSFER_ENCODING);
			this->reader = std::make_shared<QuotePrintableReader>(this->reader);
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

	return str::wstring_to_string(this->disposition_params->at(L"name"));
}

std::string Part::file_name()
{
	if (!this->disposition_params.has_value())
	{
		this->parse_content_disposition();
	}

	auto filename = str::wstring_to_string(this->disposition_params->at(L"filename"));
	if (filename.empty())
	{
		return "";
	}

	// RFC 7578, Section 4.2 requires that if a filename is provided, the
	// directory path information must not be used.
	return path::basename(filename);
}

long long int Part::read(std::string& buffer, long long int max_n)
{
	auto total_bytes = std::min<ssize_t>(this->content_length, max_n);
	auto bytes_to_read = total_bytes;
	std::string bytes;
	while (bytes_to_read > 0)
	{
		auto n = this->reader->read(bytes, bytes_to_read);
		bytes_to_read -= n;
		buffer += bytes;
	}

	return total_bytes;
}

void Part::parse_content_disposition()
{
	auto v = this->get_header(CONTENT_DISPOSITION, "");
	auto [disposition_string, disposition_map, ok] = core::mime::parse_media_type(str::string_to_wstring(v));
	this->disposition = disposition_string;
	if (!ok)
	{
		this->disposition_params = disposition_map;
	}
	else
	{
		this->disposition_params = std::map<std::wstring, std::wstring>{};
	}
}

void Part::populate_headers()
{
	net::parse_headers(this->header, this->multipart_reader->buffer_reader.get(), -1);
}

std::unique_ptr<Form> Reader::read_form(long long int max_memory)
{
	auto form = std::make_unique<Form>();

	// Reserve an additional 10 MB for non-file parts.
	auto max_value_bytes = max_memory + (10 << 20);
	while (true)
	{
		auto part = this->next_part();
		if (!part)
		{
			// EOF
			break;
		}

		auto name = part->form_name();
		if (name.empty())
		{
			continue;
		}

		std::string buffer;
		auto filename = part->file_name();
		if (filename.empty())
		{
			// value, store as string in memory
			auto n = part->read(buffer, max_value_bytes + 1);
			max_value_bytes -= n;
			if (max_value_bytes < 0)
			{
				throw exc::PayloadTooLarge("message is too large", _ERROR_DETAILS_);
			}

			form->values.add(name, buffer);
			continue;
		}

		// file, store in memory or on disk
		auto fh = std::make_shared<FileHeader>();
		fh->filename = filename;
		fh->header = part->header;
		auto n = part->read(buffer, max_memory + 1);
		if (n > max_memory)
		{
			// too big, write to disk and flush buffer
			auto size = n;
			auto file = path::temp_file("", "multipart-", true);
			if (file->is_open())
			{
				try
				{
					file->write(buffer);
					while (true)
					{
						auto n_ = part->read(buffer, max_memory + 1);
						size += n_;
						if (n_ == 0)
						{
							// EOF
							break;
						}
					}

					file->save();
					file->close();
				}
				catch (const FileError& exc)
				{
					if (path::exists(file->path()))
					{
						std::filesystem::remove(file->path());
					}

					throw FileError(exc);
				}
			}

			fh->tmp_file = file->path();
			fh->size = size;
		}
		else
		{
			fh->content = buffer;
			fh->size = buffer.size();
			max_memory -= n;
			max_value_bytes -= n;
		}

		form->files.add(name, std::move(fh));
	}

	return std::move(form);
}

std::shared_ptr<Part> Reader::next_part(bool raw_part)
{
//	if (this->current_part)
//	{
//		this->current_part->close();
//	}

	if (this->dash_boundary == "--")
	{
		throw ParseError("multipart: boundary is empty", _ERROR_DETAILS_);
	}

	auto expect_new_part = false;
	while (true)
	{
		std::string line;
		this->buffer_reader->read_line(line);
		if (this->is_final_boundary(line))
		{
			// EOF
			return nullptr;
		}

		if (this->is_boundary_delimiter_line(line))
		{
			this->parts_read++;
			std::string remaining_bytes;
			if (this->current_part)
			{
				remaining_bytes = this->current_part->remaining_bytes;
			}

			auto bp = std::make_shared<Part>(this, this->content_length, raw_part, remaining_bytes);
			this->current_part = bp;
			return bp;
		}

		if (this->is_final_boundary(line))
		{
			// EOF
			return nullptr;
		}

		if (expect_new_part)
		{
			throw ParseError("multipart: expecting a new Part; got line " + line, _ERROR_DETAILS_);
		}

		if (this->parts_read == 0)
		{
			// skip line
			continue;
		}

		// Consume the "\n" or "\r\n" separator between the
		// body of the previous part and the boundary line we
		// now expect will follow. (either a new part or the
		// end boundary)
		if (line == this->nl)
		{
			expect_new_part = true;
			continue;
		}

		throw ParseError("multipart: unexpected line in next_part(): " + line);
	}
}

bool Reader::is_final_boundary(const std::string& line)
{
	if (!line.starts_with(this->dash_boundary_dash))
	{
		return false;
	}

	auto rest = line.substr(this->dash_boundary_dash.size());
	rest = skip_lwsp_char(rest);
	return rest.empty() || rest == this->nl;
}

bool Reader::is_boundary_delimiter_line(const std::string& line)
{
	// https://tools.ietf.org/html/rfc2046#section-5.1
	//   The boundary delimiter line is then defined as a line
	//   consisting entirely of two hyphen characters ("-",
	//   decimal value 45) followed by the boundary parameter
	//   value from the Content-Type header field, optional linear
	//   whitespace, and a terminating CRLF.
	if (!line.starts_with(this->dash_boundary))
	{
		return false;
	}

	auto rest = line.substr(this->dash_boundary.size());
	rest = skip_lwsp_char(rest);

	// On the first part, see our lines are ending in \n instead of \r\n
	// and switch into that mode if so. This is a violation of the spec,
	// but occurs in practice.
	if (this->parts_read == 0 && rest.size() == 1 && rest[0] == '\n')
	{
		this->nl = this->nl.substr(1);
		this->nl_dash_boundary = this->nl_dash_boundary.substr(1);
	}

	return rest == this->nl;
}

ssize_t PartReader::read(std::string& buffer, size_t max_count)
{
	auto mr = this->part->multipart_reader;
	std::string bytes;
	mr->buffer_reader->read(bytes, max_count);
	bytes = this->remaining_bytes + bytes;
	auto [n, ok] = _scan_until_boundary(bytes, mr->dash_boundary, mr->nl_dash_boundary, this->part->total);
	if (n == 0)
	{
		return 0;
	}

	buffer = bytes.substr(0, n);
	this->remaining_bytes = bytes.substr(n);
	this->part->total += n;
	return n;
}

std::pair<ssize_t, bool> _scan_until_boundary(
	const std::string& buf, const std::string& dash_boundary, const std::string& nl_dash_boundary, ssize_t total
)
{
	if (total == 0)
	{
		// At beginning of body, allow dash_boundary.
		if (buf.starts_with(dash_boundary))
		{
			switch (_match_after_prefix(buf, dash_boundary))
			{
				case -1:
					return {dash_boundary.size(), true};
				case 0:
					return {0, true};
				case +1:
					return {0, false};
			}
		}

		if (dash_boundary.starts_with(buf))
		{
			return {0, false};
		}
	}

	// Search for "\n--boundary".
	auto i = buf.find(nl_dash_boundary);
	if (i != std::string::npos)
	{
		switch (_match_after_prefix(buf.substr(i), nl_dash_boundary))
		{
			case -1:
				return {i + nl_dash_boundary.size(), true};
			case 0:
				return {i, true};
			case +1:
				return {i, false};
		}
	}

	if (nl_dash_boundary.starts_with(buf))
	{
		return {0, false};
	}

	// Otherwise, anything up to the final \n is not part of the boundary
	// and so must be part of the body.
	// Also if the section from the final \n onward is not a prefix of the boundary,
	// it too must be part of the body.
	i = buf.find_last_of(nl_dash_boundary[0]);
	if (i != std::string::npos && nl_dash_boundary.starts_with(buf.substr(i)))
	{
		return {i, true};
	}

	return {buf.size(), false};
}

ssize_t _match_after_prefix(const std::string& buf, const std::string& prefix)
{
	if (buf.size() == prefix.size())
	{
		return 0;
	}

	auto c = buf[prefix.size()];
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '-')
	{
		return +1;
	}

	return -1;
}

__HTTP_MULTIPART_END__
