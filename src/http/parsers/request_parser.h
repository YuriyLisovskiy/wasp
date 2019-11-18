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

/*
 * http_parser definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_PARSERS_REQUEST_PARSER_H
#define WASP_HTTP_PARSERS_REQUEST_PARSER_H

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <strings.h>

#include "../../globals.h"
#include "../request.h"
#include "multipart_parser.h"
#include "query_parser.h"
#include "cookie_parser.h"
#include "../../utils/str.h"


__INTERNAL_BEGIN__

struct request_parser final
{
	size_t major_v{};
	size_t minor_v{};
	std::string path;
	std::string query;
	std::string method;
	bool keep_alive{};
	std::string content;
	std::map<std::string, std::string> headers;
	HttpRequest::Parameters<std::string, std::string>* get_parameters = nullptr;
	HttpRequest::Parameters<std::string, std::string>* post_parameters = nullptr;
	HttpRequest::Parameters<std::string, UploadedFile>* files_parameters = nullptr;

	unsigned long long content_size{};
	std::string chunk_size_str;
	unsigned long long chunk_size{};
	bool chunked{};

	// Used only for POST, PUT or PATCH methods type
	enum content_type_enum
	{
		ct_application_x_www_form_url_encoded,
		ct_application_json,
		ct_multipart_form_data,
		ct_other

	} content_type{};

	enum state_enum
	{
		s_method_begin,
		s_method,
		s_path_begin,
		s_path,
		s_query,
		s_fragment,
		s_http_version_h,
		s_http_version_ht,
		s_http_version_htt,
		s_http_version_http,
		s_http_version_slash,
		s_http_version_major_begin,
		s_http_version_major,
		s_http_version_minor_begin,
		s_http_version_minor,
		s_http_version_new_line,

		s_header_line_start,
		s_header_lws,
		s_header_name,
		s_header_space_before_value,
		s_header_value,

		s_expecting_new_line_2,
		s_expecting_new_line_3,

		s_request_body,

		s_chunk_size,
		s_chunk_extension_name,
		s_chunk_extension_value,
		s_chunk_size_new_line,
		s_chunk_size_new_line_2,
		s_chunk_size_new_line_3,
		s_chunk_trailer_name,
		s_chunk_trailer_value,

		s_chunk_data_new_line_1,
		s_chunk_data_new_line_2,
		s_chunk_data

	} state{};

	void parse_http_word(char input, char expected, request_parser::state_enum new_state);

	// May be overloaded for custom parser which is inherited from HttpRequestParser
	virtual void set_parameters(HttpRequest::Parameters<std::string, std::string>* params);

	void parse_body(const std::string& data);

	// Helpers
	// Check if a byte is an HTTP character.
	static bool is_char(uint c);

	// Check if a byte is an HTTP control character.
	static bool is_control(uint c);

	// Check if a byte is defined as an HTTP special character.
	static bool is_special(uint c);

	// Check if a byte is a digit.
	static bool is_digit(uint c);

	request_parser() = default;
	~request_parser();

	wasp::HttpRequest build_request();
	Dict<std::string, std::string> get_headers();
	void parse_body(const std::string& data, const std::string& media_root);
	void parse_headers(const std::string& data);
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_REQUEST_PARSER_H
