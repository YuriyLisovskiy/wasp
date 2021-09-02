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
	std::string remaining_bytes;

	explicit Part(Reader* reader, ssize_t content_length, bool raw_part, std::string remaining_bytes);

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

	long long int read(std::string& buffer, long long int max_n);

protected:
	friend class PartReader;

	ssize_t content_length = 0;
	Reader* multipart_reader = nullptr;

	// It is either a reader directly reading from `multipart_reader`, or it's a
	// wrapper around such a reader, decoding the Content-Transfer-Encoding
	std::shared_ptr<io::IReader> reader = nullptr;

	std::wstring disposition;
	std::optional<std::map<std::wstring, std::wstring>> disposition_params;

	// total data bytes read already
	ssize_t total = 0;

	void parse_content_disposition();

	void populate_headers();
};

// TESTME: Reader
// TODO: docs for 'Reader'
class Reader
{
public:
	inline Reader(std::shared_ptr<io::IReader> reader, const std::string& boundary, ssize_t content_length) :
		buffer_reader(std::move(reader)), content_length(content_length)
	{
	}

	[[nodiscard]]
	inline ssize_t get_content_length() const
	{
		return this->content_length;
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

	ssize_t content_length;
	std::shared_ptr<io::IReader> buffer_reader;

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
};

// TESTME: PartReader
//
// Implements `io::IReader` by reading raw bytes directly from the
// wrapped `Part*`, without doing any Transfer-Encoding decoding.
class PartReader : public io::IReader
{
public:
	inline explicit PartReader(Part* part, std::string remaining_bytes) :
		part(part), remaining_bytes(std::move(remaining_bytes))
	{
		if (!this->part)
		{
			throw NullPointerException("'part' is nullptr", _ERROR_DETAILS_);
		}
	}

	inline ssize_t read_line(std::string& buffer) override
	{
		throw NotImplementedException("PartReader::read_line(std::string& buffer)", _ERROR_DETAILS_);
	}

	ssize_t read(std::string& buffer, size_t max_count) override;

	inline bool close_reader() override
	{
		throw NotImplementedException("PartReader::close_reader()", _ERROR_DETAILS_);
	}

protected:
	Part* part;
	std::string remaining_bytes;
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

// TESTME: _scan_until_boundary
//
// Scans `buf` to identify how much of it can be safely
// returned as part of the `Part` body.
// `dash_boundary` is "--boundary".
// `nl_dash_boundary` is "\r\n--boundary" or "\n--boundary", depending on what mode we are in.
// The comments below (and the name) assume "\n--boundary", but either is accepted.
// total is the number of bytes read out so far. If total == 0, then a leading "--boundary" is recognized.
extern std::pair<ssize_t, bool> _scan_until_boundary(
	const std::string& buf, const std::string& dash_boundary, const std::string& nl_dash_boundary, ssize_t total
);

// TESTME: _match_after_prefix
//
// Checks whether `buf` should be considered to match the boundary.
// The `prefix` is "--boundary" or "\r\n--boundary" or "\n--boundary",
// and the caller has verified already that `buf.starts_with(prefix)` is true.
//
// Returns +1 if the buffer does match the boundary,
// meaning the prefix is followed by a dash, space, tab, cr, nl, or end of input.
// It returns -1 if the buffer definitely does NOT match the boundary,
// meaning the `prefix` is followed by some other character.
// For example, "--foobar" does not match "--foo".
// It returns 0 more input needs to be read to make the decision,
// meaning that `buf.size() == prefix.size()`.
extern ssize_t _match_after_prefix(const std::string& buf, const std::string& prefix);

__HTTP_MULTIPART_END__
