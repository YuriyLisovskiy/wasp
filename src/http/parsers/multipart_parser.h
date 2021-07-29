/**
 * http/parsers/multipart_parser.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * multipart/form-data parser.
 */

#pragma once

// Base libraries.
#include <xalwart.base/collections/dictionary.h>
#include <xalwart.base/collections/multi_dictionary.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../../core/uploaded_file.h"


__HTTP_INTERNAL_BEGIN__

// TESTME: multipart_parser
struct multipart_parser final
{
	std::string media_root;

	collections::Dictionary<std::string, std::string> post_values;
	collections::MultiDictionary<std::string, std::string> multi_post_value;

	collections::Dictionary<std::string, files::UploadedFile> file_values;
	collections::MultiDictionary<std::string, files::UploadedFile> multi_file_value;

	enum state
	{
		s_boundary_begin,
		s_boundary,
		s_boundary_end,
		s_body_end,
		s_content_disposition_begin,
		s_content_disposition,
		s_name_begin,
		s_name,
		s_name_end,
		s_file_name_begin,
		s_file_name,
		s_content_type_begin,
		s_content_type,
		s_content_begin,
		s_content

	};

	// Constructs parser from media root.
	//
	// `media_root`: path where file should be saved.
	//
	// !IMPORTANT! The parser does not save file, but
	// only builds path to future file location.
	explicit multipart_parser(const std::string& media_root="");

	// Appends non-file parameter to the result.
	//
	// `key`: name of the parameter.
	// `value`: actual value of the parameter.
	void append_parameter(const std::string& key, const std::string& value);

	// Creates `UploadedFile` object from given bytes and
	// meta information and appends it to the result.
	//
	// `key`: name of the parameter.
	// `file_name`: name of parsed file.
	// `content_type`: content type of the file.
	// `boundary`: boundary name.
	// `content_disposition`: content disposition string.
	// `data`: actual content of the file in bytes.
	void append_file(
		const std::string& key,
		const std::string& file_name,
		const std::string& content_type,
		const std::string& boundary,
		const std::string& content_disposition,
		std::vector<unsigned char>& data
	);

	// Retrieves boundary name from Content-Type header.
	//
	// Throws `core::MultiPartParserError` if Content-Type
	// is not `multipart` or if retrieved boundary is empty.
	//
	// `content_type`: value of Content-Type header.
	static std::string get_boundary(const std::string& content_type);

	// Checks if boundaries are equal.
	//
	// Throws `core::MultiPartParserError` if boundaries are
	// different.
	static void assert_boundary(const std::string& actual, const std::string& expected);

	// Parses input data. Extracts request's parameters.
	//
	// `content_type`: data content type.
	// `body`: input data to parse.
	void parse(const std::string& content_type, const std::string& body);
};

__HTTP_INTERNAL_END__
