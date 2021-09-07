/**
 * http/mime/multipart.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Multipart request parser utilities.
 *
 * Implementation is based on Golang 1.15.8: mime/multipart/formdata.go
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>
#include <map>

// Base libraries.
#include <xalwart.base/io.h>
#include <xalwart.base/collections/multi_dictionary.h>
#include <xalwart.base/path.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../core/uploaded_file.h"


__HTTP_MULTIPART_BEGIN__

const long long int DEFAULT_MAX_MEMORY = 32 << 20; // 32 MB

// TESTME: FileHeader
// Describes a file part of a multipart request.
class FileHeader final
{
public:
	std::string filename;
	std::map<std::string, std::string> header;
	unsigned long long int size;

	std::string content;
	std::string tmp_file;

	// Opens and returns the `FileHeader`'s associated `UploadedFile`.
	[[nodiscard]]
	files::UploadedFile open() const;
};

// TESTME: Form
//
// It is a parsed multipart form.
// Its `files` parts are stored either in memory or on disk,
// and are accessible via the `FileHeader`'s `open` method.
// Its `values` parts are stored as strings.
// Both are keyed by field name.
struct Form
{
	collections::MultiDictionary<std::string, std::string> values;
	collections::MultiDictionary<std::string, std::shared_ptr<FileHeader>> files;

	// Removes any temporary files associated with a `Form`.
	inline void remove_all() const;
};

class Reader;
class PartReader;

// TESTME: Part
// Represents a single part in a multipart body.
class Part
{
public:
	std::map<std::string, std::string> header;
	Reader* multipart_reader = nullptr;
	ssize_t n = 0;

	// total data bytes read already
	ssize_t total = 0;

	explicit Part(
		Reader* reader, ssize_t remaining_content_length, bool raw_part,
		ssize_t max_header_length, ssize_t max_headers_count
	);

	// Returns the name parameter if p has a Content-Disposition
	// of type "form-data". Otherwise it returns the empty string.
	std::string form_name();

	// Returns the filename parameter of the `Part`'s Content-Disposition
	// header. If not empty, the filename is passed through `xw::path::basename` (which is
	// platform dependent) before being returned.
	std::string file_name();

	[[nodiscard]]
	inline std::string get_header(const std::string& key, const std::string& default_value) const
	{
		return this->header.contains(key) ? this->header.at(key) : default_value;
	}

	inline void remove_header(const std::string& key)
	{
		if (this->header.contains(key))
		{
			this->header.erase(key);
		}
	}

	ssize_t read(std::string& buffer, size_t max_count);

	[[nodiscard]]
	inline ssize_t get_total() const
	{
		return this->total;
	}

protected:
	ssize_t max_headers_count;
	ssize_t max_header_length;

	ssize_t remaining_content_length = 0;

	// It is either a reader directly reading from `multipart_reader`, or it's a
	// wrapper around such a reader, decoding the Content-Transfer-Encoding
	std::shared_ptr<io::IReader> reader = nullptr;

	std::wstring disposition;
	std::optional<std::map<std::wstring, std::wstring>> disposition_params;

	void parse_content_disposition();

	void populate_headers();
};

// TESTME: Reader
// TODO: docs for 'Reader'
class Reader
{
public:
	inline Reader(
		std::shared_ptr<io::IBufferedReader> reader, const std::string& boundary, ssize_t content_length,
		ssize_t max_file_upload_size, ssize_t max_fields_count, ssize_t max_header_length, ssize_t max_headers_count
	) :
		buffer_reader(std::move(reader)), remaining_content_length(content_length),
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

protected:
	friend class Part;
	friend class PartReader;

	ssize_t max_file_upload_size;
	ssize_t max_fields_count;
	ssize_t max_header_length;
	ssize_t max_headers_count;
	ssize_t remaining_content_length;
	std::shared_ptr<io::IBufferedReader> buffer_reader;

	std::shared_ptr<Part> current_part;
	int parts_read{};

	std::string nl;                 // "\r\n" or "\n" (set after seeing first boundary line)
	std::string nl_dash_boundary;   // nl + "--boundary"
	std::string dash_boundary_dash; // "--boundary--"
	std::string dash_boundary;      // "--boundary"

	std::shared_ptr<Part> next_part(bool raw_part);

	// Reports whether line is the final boundary line
	// indicating that all parts are over.
	// It matches `^--boundary--[ \t]*(\r\n)?$`
	bool is_final_boundary(const std::string& line);

	bool is_boundary_delimiter_line(const std::string& line);

	void check_part_upload_size(Part* part, FileHeader* header) const;
};

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

__HTTP_MULTIPART_END__
