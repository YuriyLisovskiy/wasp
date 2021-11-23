/**
 * http/mime/multipart/part.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Single part of the body.
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
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"


__HTTP_MIME_MULTIPART_BEGIN__

class BodyReader;

// TESTME: Part
// Represents a single part in a multipart body.
struct Part final
{
	ssize_t max_headers_count;
	ssize_t max_header_length;

	ssize_t remaining_content_length = 0;

	// It is either a reader directly reading from `multipart_reader`, or it's a
	// wrapper around such a reader, decoding the Content-Transfer-Encoding
	std::shared_ptr<io::IReader> reader = nullptr;

	std::wstring disposition;
	std::optional<std::map<std::wstring, std::wstring>> disposition_params;

	std::map<std::string, std::string> header;
	BodyReader* multipart_reader = nullptr;
	ssize_t n = 0;

	// total data bytes read already
	ssize_t total = 0;

	explicit Part(
		BodyReader* reader, ssize_t remaining_content_length, bool raw_part,
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

	void parse_content_disposition();

	void populate_headers();
};

__HTTP_MIME_MULTIPART_END__
