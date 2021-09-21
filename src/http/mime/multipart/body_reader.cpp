/**
 * http/mime/multipart/body_reader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./body_reader.h"

// C++ libraries.
#include <filesystem>

// Base libraries.
#include <xalwart.base/path.h>

// Framework libraries.
#include "../../exceptions.h"


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: skip_lwsp_char
// Returns `s` with leading spaces and tabs removed.
// RFC 822 defines:
//    LWSP-char = SPACE / HTAB
inline std::string skip_lwsp_char(std::string s)
{
	while (!s.empty() && (s[0] == ' ' || s[0] == '\t'))
	{
		s = s.substr(1);
	}

	return s;
}

Form BodyReader::read_form(long long int max_memory)
{
	Form form;

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
			form.values.add(name, buffer);
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

		form.files.add(name, std::move(fh));
	}

	return form;
}

std::shared_ptr<Part> BodyReader::next_part(bool raw_part)
{
	if (this->dash_boundary == "--")
	{
		throw ParseError("multipart: boundary is empty", _ERROR_DETAILS_);
	}

	auto expect_new_part = false;
	while (true)
	{
		std::string line;
		this->remaining_content_length -= this->buffered_reader->read_line(line);
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

bool BodyReader::is_final_boundary(const std::string& line) const
{
	if (!line.starts_with(this->dash_boundary_dash))
	{
		return false;
	}

	auto rest = line.substr(this->dash_boundary_dash.size());
	rest = skip_lwsp_char(rest);
	return rest.empty() || rest == this->nl;
}

bool BodyReader::is_boundary_delimiter_line(const std::string& line)
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

void BodyReader::check_part_upload_size(Part* part, FileHeader* header) const
{
	if (part->total > this->max_file_upload_size)
	{
		throw exc::PayloadTooLarge("file too large: '" + header->filename + "'", _ERROR_DETAILS_);
	}
}

__HTTP_MIME_MULTIPART_END__
