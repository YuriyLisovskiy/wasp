/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * multipart_parser.h
 * Purpose: parses multipart/form-data http request.
 */

#ifndef WASP_CORE_PARSERS_MULTIPART_PARSER_H
#define WASP_CORE_PARSERS_MULTIPART_PARSER_H

// C++ libraries.
#include <string>
#include <functional>
#include <fstream>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../../utility/string/str.h"
#include "../../core/exceptions.h"
#include "../../collections/dict.h"
#include "../../collections/multi_dict.h"
#include "../../core/files/file.h"
#include "../../core/files/uploaded_file.h"
#include "../../http/request.h"


__CORE_INTERNAL_BEGIN__

struct multipart_parser final
{
	std::string media_root;

	collections::Dict<std::string, std::string> post_values;
	collections::MultiValueDict<std::string, std::string> multi_post_value;

	collections::Dict<std::string, UploadedFile> file_values;
	collections::MultiValueDict<std::string, UploadedFile> multi_file_value;

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

	void append_parameter(const std::string& key, const std::string& value);
	void append_file(
		const std::string& key,
		const std::string& file_name,
		const std::string& content_type,
		const std::string& boundary,
		const std::string& content_disposition,
		const std::vector<byte>& data
	);

	static std::string get_boundary(const std::string& content_type);
	static void assert_boundary(const std::string& actual, const std::string& expected);

	explicit multipart_parser(const std::string& mediaRoot = "");
	void parse(const std::string& content_type, const std::string& body);
	http::HttpRequest::Parameters<std::string, UploadedFile>* get_files_params();
	http::HttpRequest::Parameters<std::string, std::string>* get_post_params();
};

__CORE_INTERNAL_END__


#endif // WASP_CORE_PARSERS_MULTIPART_PARSER_H