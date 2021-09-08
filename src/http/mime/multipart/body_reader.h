/**
 * http/mime/multipart/body_reader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Reader for splitting parts from multipart/form-data request body.
 *
 * Implementation is based on Golang 1.15.8: mime/quoteprintable/reader.go
 */

#pragma once

// C++ libraries.
#include <memory>
#include <string>

// Base libraries.
#include <xalwart.base/io.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./file_header.h"
#include "./part.h"
#include "./form.h"


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: BodyReader
// TODO: docs for 'BodyReader'
struct BodyReader final
{
	ssize_t max_file_upload_size;
	ssize_t max_fields_count;
	ssize_t max_header_length;
	ssize_t max_headers_count;
	ssize_t remaining_content_length;
	std::shared_ptr<io::ILimitedBufferedReader> buffered_reader;

	std::shared_ptr<Part> current_part;
	int parts_read{};

	std::string nl;                 // "\r\n" or "\n" (set after seeing first boundary line)
	std::string nl_dash_boundary;   // nl + "--boundary"
	std::string dash_boundary_dash; // "--boundary--"
	std::string dash_boundary;      // "--boundary"

	inline BodyReader(
		std::shared_ptr<io::ILimitedBufferedReader> reader, const std::string& boundary, ssize_t content_length,
		ssize_t max_file_upload_size, ssize_t max_fields_count, ssize_t max_header_length, ssize_t max_headers_count
	) :
		buffered_reader(std::move(reader)), remaining_content_length(content_length),
		max_file_upload_size(max_file_upload_size), max_fields_count(max_fields_count),
		max_header_length(max_header_length), max_headers_count(max_headers_count)
	{
		auto b = "\r\n--" + boundary + "--";
		this->nl = b.substr(0, 2);
		this->nl_dash_boundary = b.substr(0, b.size() - 2);
		this->dash_boundary_dash = b.substr(2);
		this->dash_boundary = b.substr(2, b.size() - 4);
	}

	std::unique_ptr<Form> read_form(long long int max_memory);

	// Returns the next part in the multipart or `nullptr` on error.
	// When there are no more parts, `nullptr` is returned.
	//
	// As a special case, if the "Content-Transfer-Encoding" header
	// has a value of "quoted-printable", that header is instead
	// hidden and the body is transparently decoded during Read calls.
	inline std::shared_ptr<Part> next_part()
	{
		return this->next_part(false);
	}

	std::shared_ptr<Part> next_part(bool raw_part);

	// Reports whether line is the final boundary line
	// indicating that all parts are over.
	// It matches `^--boundary--[ \t]*(\r\n)?$`
	[[nodiscard]]
	bool is_final_boundary(const std::string& line) const;

	bool is_boundary_delimiter_line(const std::string& line);

	void check_part_upload_size(Part* part, FileHeader* header) const;
};

__HTTP_MIME_MULTIPART_END__
