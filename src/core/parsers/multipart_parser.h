/**
 * core/parsers/multipart_parser.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: parses multipart/form-data http request.
 */

#pragma once

// Core libraries.
#include <xalwart.core/string.h>
#include <xalwart.core/collections/multi_dict.h>

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../../core/uploaded_file.h"


__PARSERS_BEGIN__

struct multipart_parser final
{
	std::string media_root;

	collections::Dict<std::string, xw::string> post_values;
	collections::MultiValueDict<std::string, xw::string> multi_post_value;

	collections::Dict<std::string, files::UploadedFile> file_values;
	collections::MultiValueDict<std::string, files::UploadedFile> multi_file_value;

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

	void append_parameter(const std::string& key, const xw::string& value);
	void append_file(
		const std::string& key,
		const std::string& file_name,
		const std::string& content_type,
		const std::string& boundary,
		const std::string& content_disposition,
		const std::vector<core::byte>& data
	);

	static std::string get_boundary(const std::string& content_type);
	static void assert_boundary(const std::string& actual, const std::string& expected);

	explicit multipart_parser(const std::string& mediaRoot = "");
	void parse(const std::string& content_type, const xw::string& body);
};

__PARSERS_END__
