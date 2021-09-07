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
#include "./part_reader.h"
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

Part::Part(
	Reader* multipart_reader, ssize_t remaining_content_length, bool raw_part,
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
	return path::basename(filename);
}

ssize_t Part::read(std::string& buffer, size_t max_count)
{
	auto bytes_to_read = std::min<ssize_t>(this->remaining_content_length, max_count);
	ssize_t total_bytes_read = 0;
	std::string bytes;
	while (bytes_to_read > 0)
	{
		auto n_ = this->reader->read(bytes, bytes_to_read);

//		std::cerr << "REM CONTENT LEN (TO READ): " << bytes_to_read << ", N: " << n_ << '\n';

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
	auto [disposition_string, disposition_map, ok] = core::mime::parse_media_type(content_disposition);
	this->disposition = disposition_string;
	this->disposition_params = ok ? disposition_map : std::map<std::wstring, std::wstring>{};
}

void Part::populate_headers()
{
	net::parse_headers(
		this->header, this->multipart_reader->buffer_reader.get(), this->max_header_length, this->max_headers_count
	);
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

			this->remaining_content_length -= n;
			form->values.add(name, buffer);
			continue;
		}

		// file, store in memory or on disk
		auto fh = std::make_shared<FileHeader>();
		fh->filename = filename;
		fh->header = part->header;
		auto n = part->read(buffer, max_memory + 1);
		this->check_part_upload_size(part.get(), fh.get());
		this->remaining_content_length -= n;
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
						this->check_part_upload_size(part.get(), fh.get());
						size += n_;
						if (n_ == 0)
						{
							// EOF
							break;
						}

						this->remaining_content_length -= n;
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
	if (this->dash_boundary == "--")
	{
		throw ParseError("multipart: boundary is empty", _ERROR_DETAILS_);
	}

	auto expect_new_part = false;
	while (true)
	{
//		std::cerr << "REM CL: " << this->remaining_content_length << '\n';

		std::string line;
		this->remaining_content_length -= this->buffer_reader->read_line(line);
		if (this->is_final_boundary(line))
		{
			// EOF
			return nullptr;
		}

		if (this->is_boundary_delimiter_line(line))
		{
			this->parts_read++;
			if (this->parts_read > this->max_fields_count)
			{
				throw exc::PayloadTooLarge("too much fields", _ERROR_DETAILS_);
			}

			auto bp = std::make_shared<Part>(
				this, this->remaining_content_length, raw_part, this->max_header_length, this->max_headers_count
			);
			this->current_part = bp;
			return bp;
		}

//		if (this->is_final_boundary(line))
//		{
			// EOF
//			return nullptr;
//		}

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

void Reader::check_part_upload_size(Part* part, FileHeader* header) const
{
	if (part->total > this->max_file_upload_size)
	{
		throw exc::PayloadTooLarge("file too large: '" + header->filename + "'", _ERROR_DETAILS_);
	}
}

__HTTP_MULTIPART_END__
