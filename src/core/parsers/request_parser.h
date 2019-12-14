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
 * request_parser.h
 * Purpose: parses an http request from given stream.
 */

#ifndef WASP_CORE_PARSERS_REQUEST_PARSER_H
#define WASP_CORE_PARSERS_REQUEST_PARSER_H

#include <algorithm>
#include <cstring>
#include <map>
#include <string>

#include "../../globals.h"
#include "../../http/request.h"
#include "multipart_parser.h"
#include "query_parser.h"
#include "../../utility/str.h"


__INTERNAL_BEGIN__

/// Http request parser structure.
struct request_parser final
{
	/// Major part of http protocol version.
	size_t major_v{};

	/// Minor part of http protocol version.
	size_t minor_v{};

	/// Request's path.
	std::string path;

	/// Contains request's query.
	/// If field is empty-string, request has not query.
	std::string query;

	/// Hold http request's method type.
	std::string method;

	/// Indicates whether request's connection is keep alive or not.
	bool keep_alive{};

	/// Contains body of http request.
	std::string content;

	/// Accumulates request's headers.
	std::map<std::string, std::string> headers;

	/// Contains get request's parameters.
	HttpRequest::Parameters<std::string, std::string>* get_parameters = nullptr;

	/// Contains post request's parameters.
	HttpRequest::Parameters<std::string, std::string>* post_parameters = nullptr;

	/// Contains request's files when request was sent as application/form-data.
	HttpRequest::Parameters<std::string, UploadedFile>* files_parameters = nullptr;

	/// Contains the size of request's content.
	unsigned long long content_size{};

	/// Contains the size of request's chunk as std::string.
	/// Used only for chunked requests.
	std::string chunk_size_str;

	/// Contains the size of request's chunk.
	/// Used only for chunked requests.
	unsigned long long chunk_size{};

	/// Indicates whether request is chunked or not.
	bool chunked{};

	/// Available content types.
	/// Used only for POST, PUT or PATCH methods type.
	enum content_type_enum
	{
		ct_application_x_www_form_url_encoded,
		ct_application_json,
		ct_multipart_form_data,
		ct_other

	/// Request's content type.
	} content_type{};

	/// Available parser states.
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

	/// Current parser state.
	} state{};

	/// Parses 'HTTP' word from http request's head line.
	///
	/// @param input: one of 'HTTP' letters from input stream.
	/// @param expected: an expected letter of 'HTTP' word.
	/// @param new_state: new parser's state will be set if 'input' equals 'expected'.
	void parse_http_word(char input, char expected, request_parser::state_enum new_state);

	/// Sets parameters according to http request method.
	///
	/// @param params: parsed get/post parameters.
	void set_parameters(HttpRequest::Parameters<std::string, std::string>* params);

	/// Parses chunks from http request body if request is chunked.
	///
	/// @param data: chunked http request body.
	void parse_chunks(const std::string& data);

	/// Checks if a byte is an HTTP character.
	///
	/// @param c: a byte of an input stream of HTTP request.
	/// @return True if checking is passed, otherwise returns false.
	static bool is_char(uint c);

	/// Checks if a byte is an HTTP control character.
	///
	/// @param c: a byte of an input stream of HTTP request.
	/// @return True if checking is passed, otherwise returns false.
	static bool is_control(uint c);

	/// Checks if a byte is defined as an HTTP special character.
	///
	/// @param c: a byte of an input stream of HTTP request.
	/// @return True if checking is passed, otherwise returns false.
	static bool is_special(uint c);

	/// Checks if a byte is a digit.
	///
	/// @param c: a byte of an input stream of HTTP request.
	/// @return True if checking is passed, otherwise returns false.
	static bool is_digit(uint c);

	/// Default constructor.
	request_parser() = default;

	/// Deletes struct's fields which are pointers.
	~request_parser();

	/// Builds an http request from parsed data.
	///
	/// @return HttpRequest object built from http stream.
	wasp::HttpRequest* build_request();

	/// Creates Dict object from headers' map.
	///
	/// @return Dict which contains http request headers.
	Dict<std::string, std::string> get_headers();

	/// Parses http request body from given stream.
	///
	/// @param data: http request body as std::string.
	/// @param media_root: path to media folder, where files will be saved;
	///	if this parameter is empty-string, files will not be saved.
	void parse_body(const std::string& data, const std::string& media_root);

	/// Parses http request headers from given stream.
	///
	/// @param data: http request headers as std::string.
	void parse_headers(const std::string& data);
};

__INTERNAL_END__


#endif // WASP_CORE_PARSERS_REQUEST_PARSER_H
