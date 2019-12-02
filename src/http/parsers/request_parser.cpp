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
 * An implementation of request_parser.h
 *
 * @author Yuriy Lisovskiy
 */

#include "request_parser.h"


__INTERNAL_BEGIN__

// Deletes struct's fields which are pointers.
request_parser::~request_parser()
{
	delete this->get_parameters;
	delete this->post_parameters;
	delete this->files_parameters;
}

// Builds an http request from parsed data.
wasp::HttpRequest request_parser::build_request()
{
	HttpRequest::Parameters<std::string, std::string> gets;
	if (this->get_parameters)
	{
		gets = *this->get_parameters;
	}

	HttpRequest::Parameters<std::string, std::string> posts;
	if (this->post_parameters)
	{
		posts = *this->post_parameters;
	}

	HttpRequest::Parameters<std::string, UploadedFile> files;
	if (this->files_parameters)
	{
		files = *this->files_parameters;
	}

	wasp::HttpRequest request(
		this->method,
		this->path,
		this->major_v,
		this->minor_v,
		this->query,
		this->keep_alive,
		this->content,
		this->headers,
		gets,
		posts,
		files
	);
	return request;
}

// Creates Dict object from headers' map.
Dict<std::string, std::string> request_parser::get_headers()
{
	return Dict<std::string, std::string>(this->headers);
}

// Parses http request headers from given stream.
void request_parser::parse_headers(const std::string& data)
{
	std::string newHeaderName;
	std::string newHeaderValue;

	std::map<std::string, std::string>::iterator connectionIt;

	for (const char& input : data)
	{
		switch (this->state)
		{
			case request_parser::state_enum::s_method_begin:
				if (!request_parser::is_char(input) || request_parser::is_control(input) || request_parser::is_special(input))
				{
					throw ParseError("unable to parse method type", _ERROR_DETAILS_);
				}
				else
				{
					this->state = request_parser::state_enum::s_method;
					this->method.push_back(input);
				}
				break;
			case request_parser::state_enum::s_method:
				if (input == ' ')
				{
					this->state = request_parser::state_enum::s_path_begin;
				}
				else if (!request_parser::is_char(input) || request_parser::is_control(input) || request_parser::is_special(input))
				{
					throw ParseError("unable to parse http method type", _ERROR_DETAILS_);
				}
				else
				{
					this->method.push_back(input);
				}
				break;
			case request_parser::state_enum::s_path_begin:
				if (request_parser::is_control(input))
				{
					throw ParseError("unable to parse http url path", _ERROR_DETAILS_);
				}
				else
				{
					this->state = request_parser::state_enum::s_path;
					this->path.push_back(input);
				}
				break;
			case request_parser::state_enum::s_path:
				if (input == ' ')
				{
					this->state = request_parser::state_enum::s_http_version_h;
				}
				else if (input == '?')
				{
					this->state = request_parser::state_enum::s_query;
				}
				else if (input == '\r')
				{
					this->major_v = 0;
					this->minor_v = 9;
					return;
				}
				else if (request_parser::is_control(input))
				{
					throw ParseError("unable to parse http url path", _ERROR_DETAILS_);
				}
				else
				{
					this->path.push_back(input);
				}
				break;
			case request_parser::state_enum::s_query:
				if (input == ' ')
				{
					this->state = request_parser::state_enum::s_http_version_h;
				}
				else if (input == '#')
				{
					this->state = request_parser::state_enum::s_fragment;
				}
				else if (input == '\r')
				{
					this->major_v = 0;
					this->minor_v = 9;

					return;
				}
				else if (request_parser::is_control(input))
				{
					throw ParseError("unable to parse http url query", _ERROR_DETAILS_);
				}
				else
				{
					this->query.push_back(input);
				}
				break;
			case request_parser::state_enum::s_fragment:
				if (input == ' ')
				{
					this->state = request_parser::state_enum::s_http_version_h;
				}
				// INFO: save fragment maybe.
				break;
			case request_parser::state_enum::s_http_version_h:
				this->parse_http_word(input, 'H', request_parser::state_enum::s_http_version_ht);
				break;
			case request_parser::state_enum::s_http_version_ht:
				this->parse_http_word(input, 'T', request_parser::state_enum::s_http_version_htt);
				break;
			case request_parser::state_enum::s_http_version_htt:
				this->parse_http_word(input, 'T', request_parser::state_enum::s_http_version_http);
				break;
			case request_parser::state_enum::s_http_version_http:
				this->parse_http_word(input, 'P', request_parser::state_enum::s_http_version_slash);
				break;
			case request_parser::state_enum::s_http_version_slash:
				if (input == '/')
				{
					this->major_v = 0;
					this->minor_v = 0;
					this->state = request_parser::state_enum::s_http_version_major_begin;
				}
				else
				{
					throw ParseError("unable to parse http protocol version", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_http_version_major_begin:
				if (request_parser::is_digit(input))
				{
					this->major_v = input - '0';
					this->state = request_parser::state_enum::s_http_version_major;
				}
				else
				{
					throw ParseError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_http_version_major:
				if (input == '.')
				{
					this->state = request_parser::state_enum::s_http_version_minor_begin;
				}
				else if (request_parser::is_digit(input))
				{
					this->major_v = this->major_v * 10 + input - '0';
				}
				else
				{
					throw ParseError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_http_version_minor_begin:
				if (request_parser::is_digit(input))
				{
					this->minor_v = input - '0';
					this->state = request_parser::state_enum::s_http_version_minor;
				}
				else
				{
					throw ParseError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_http_version_minor:
				if (input == '\r')
				{
					this->state = request_parser::state_enum::s_http_version_new_line;
				}
				else if(request_parser::is_digit(input))
				{
					this->minor_v = this->minor_v * 10 + input - '0';
				}
				else
				{
					throw ParseError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_http_version_new_line:
				if (input == '\n')
				{
					this->state = request_parser::state_enum::s_header_line_start;
				}
				else
				{
					throw ParseError("unable to parse http protocol main data", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_header_line_start:
				if (input == '\r')
				{
					this->state = request_parser::state_enum::s_expecting_new_line_3;
				}
				else if (!this->headers.empty() && (input == ' ' || input == '\t'))
				{
					this->state = request_parser::state_enum::s_header_lws;
				}
				else if(!request_parser::is_char(input) || request_parser::is_control(input) || request_parser::is_special(input))
				{
					throw ParseError("unable to parse http request header", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderName.reserve(16);
					newHeaderValue.reserve(16);
					newHeaderName.push_back(input);
					this->state = request_parser::state_enum::s_header_name;
				}
				break;
			case request_parser::state_enum::s_header_lws:
				if(input == '\r')
				{
					this->state = request_parser::state_enum::s_expecting_new_line_2;
				}
				else if(input == ' ' || input == '\t')
				{
				}
				else if (request_parser::is_control(input))
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				else
				{
					this->state = request_parser::state_enum::s_header_value;
					newHeaderValue.push_back(input);
				}
				break;
			case request_parser::state_enum::s_header_name:
				if (input == ':')
				{
					this->state = request_parser::state_enum::s_header_space_before_value;
				}
				else if (!request_parser::is_char(input) || request_parser::is_control(input) || request_parser::is_special(input))
				{
					throw ParseError("unable to parse http request header name", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderName.push_back(input);
				}
				break;
			case request_parser::state_enum::s_header_space_before_value:
				if (input == ' ')
				{
					this->state = request_parser::state_enum::s_header_value;
				}
				else
				{
					throw ParseError("unable to parse http request header", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_header_value:
				if (input == '\r')
				{
					if (strcasecmp(newHeaderName.c_str(), "Content-Length") == 0)
					{
						this->content_size = strtol(newHeaderValue.c_str(), nullptr, 0);
						this->content.reserve(this->content_size);
					}
					else if (strcasecmp(newHeaderName.c_str(), "Transfer-Encoding") == 0)
					{
						if (strcasecmp(newHeaderValue.c_str(), "chunked") == 0)
						{
							this->chunked = true;
						}
					}

					this->headers[newHeaderName] = newHeaderValue;
					newHeaderName.clear();
					newHeaderValue.clear();
					this->state = request_parser::state_enum::s_expecting_new_line_2;
				}
				else if (request_parser::is_control(input))
				{
					throw ParseError("unable to parse http request header value", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderValue.push_back(input);
				}
				break;
			case request_parser::state_enum::s_expecting_new_line_2:
				if (input == '\n')
				{
					this->state = request_parser::state_enum::s_header_line_start;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_expecting_new_line_3:
				connectionIt = std::find_if(
					this->headers.begin(),
					this->headers.end(),
					[](const std::pair<std::string, std::string>& item) -> bool
					{
						return strcasecmp(item.first.c_str(), "Connection") == 0;
					}
				);
				if (connectionIt != this->headers.end())
				{
					this->keep_alive = strcasecmp((*connectionIt).second.c_str(), "Keep-Alive") == 0;
				}
				else
				{
					if (this->major_v > 1 || (this->major_v == 1 && this->minor_v == 1))
					{
						this->keep_alive = true;
					}
				}

				if (this->chunked)
				{
					this->state = request_parser::state_enum::s_chunk_size;
				}
				else if (this->content_size == 0)
				{
					if (input != '\n')
					{
						throw ParseError("unable to parse http request", _ERROR_DETAILS_);
					}
				}
				else
				{
					auto contentType = this->headers["Content-Type"];
					if (contentType.find("multipart/form-data") != std::string::npos)
					{
						this->content_type = request_parser::content_type_enum::ct_multipart_form_data;
					}
					else if (contentType.find("application/json") != std::string::npos)
					{
						this->content_type = request_parser::content_type_enum::ct_application_json;
					}
					else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
					{
						this->content_type = request_parser::content_type_enum::ct_application_x_www_form_url_encoded;
					}
					else
					{
						this->content_type = request_parser::content_type_enum::ct_other;
					}
					this->state = request_parser::state_enum::s_request_body;
				}
				return;
			default:
				throw ParseError("unable to parse http request", _ERROR_DETAILS_);
		}
	}

	throw ParseError("unable to parse http request", _ERROR_DETAILS_);
}

// Parses http request body from given stream.
void request_parser::parse_body(const std::string& data, const std::string& media_root)
{
	query_parser qp;
	multipart_parser mp(media_root);
	if (data.empty())
	{
		qp.parse(this->query);
		this->set_parameters(
			new HttpRequest::Parameters<std::string, std::string>(*qp.dict, *qp.multi_dict)
		);
	}
	else
	{
		if (this->state == request_parser::state_enum::s_request_body)
		{
			this->content = data;
		}
		else
		{
			this->parse_chunks(data);
		}

		if (!this->content.empty())
		{
			switch (this->content_type)
			{
				case request_parser::content_type_enum::ct_application_x_www_form_url_encoded:
					qp.parse(this->content);
					this->set_parameters(
						new HttpRequest::Parameters<std::string, std::string>(*qp.dict, *qp.multi_dict)
					);
					break;
				case request_parser::content_type_enum::ct_application_json:
					// TODO: parse application/json data
					break;
				case request_parser::content_type_enum::ct_multipart_form_data:
					mp.parse(this->headers["Content-Type"], this->content);
					this->post_parameters = mp.get_post_params();
					this->files_parameters = mp.get_files_params();
					break;
				case request_parser::content_type_enum::ct_other:
					break;
				default:
					throw ParseError("Unknown content type", _ERROR_DETAILS_);
			}
		}
	}
}

// Parses 'HTTP' word from http request's head line.
void request_parser::parse_http_word(char input, char expected, request_parser::state_enum new_state)
{
	if (input == expected)
	{
		this->state = new_state;
	}
	else
	{
		throw ParseError("unable to parse http protocol version", _ERROR_DETAILS_);
	}
}

// Sets parameters according to http request method.
void request_parser::set_parameters(HttpRequest::Parameters<std::string, std::string>* params)
{
	if (this->method == "GET")
	{
		this->get_parameters = params;
	}
	else if (this->method == "POST")
	{
		this->post_parameters = params;
	}
}

// Parses chunks from http request body if request is chunked.
void request_parser::parse_chunks(const std::string& data)
{
	for (const char& input : data)
	{
		switch (this->state)
		{
			case request_parser::state_enum::s_chunk_size:
				if (isalnum(input))
				{
					this->chunk_size_str.push_back(input);
				}
				else if (input == ';')
				{
					this->state = request_parser::state_enum::s_chunk_extension_name;
				}
				else if (input == '\r')
				{
					this->state = request_parser::state_enum::s_chunk_size_new_line;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_extension_name:
				if (isalnum(input) || input == ' ')
				{
					// skip
				}
				else if (input == '=')
				{
					this->state = request_parser::state_enum::s_chunk_extension_value;
				}
				else if (input == '\r')
				{
					this->state = request_parser::state_enum::s_chunk_size_new_line;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_extension_value:
				if (std::isalnum(input) || input == ' ')
				{
					// skip
				}
				else if (input == '\r')
				{
					this->state = request_parser::state_enum::s_chunk_size_new_line;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_size_new_line:
				if (input == '\n')
				{
					this->chunk_size = strtol(this->chunk_size_str.c_str(), nullptr, 16);
					this->chunk_size_str.clear();
					this->content.reserve(this->content.size() + this->chunk_size);

					if (this->chunk_size == 0)
					{
						this->state = request_parser::state_enum::s_chunk_size_new_line_2;
					}
					else
					{
						this->state = request_parser::state_enum::s_chunk_data;
					}
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_size_new_line_2:
				if (input == '\r')
				{
					this->state = request_parser::state_enum::s_chunk_size_new_line_3;
				}
				else if( isalpha(input) )
				{
					this->state = request_parser::state_enum::s_chunk_trailer_name;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_size_new_line_3:
				if (input == '\n')
				{
					return;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
			case request_parser::state_enum::s_chunk_trailer_name:
				if (std::isalnum(input))
				{
					// skip
				}
				else if (input == ':')
				{
					this->state = request_parser::state_enum::s_chunk_trailer_value;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_trailer_value:
				if (std::isalnum(input) || input == ' ')
				{
					// skip
				}
				else if( input == '\r' )
				{
					this->state = request_parser::state_enum::s_chunk_size_new_line;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_data:
				this->content.push_back(input);
				if (--this->chunk_size == 0)
				{
					this->state = request_parser::state_enum::s_chunk_data_new_line_1;
				}
				break;
			case request_parser::state_enum::s_chunk_data_new_line_1:
				if (input == '\r')
				{
					this->state = request_parser::state_enum::s_chunk_data_new_line_2;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case request_parser::state_enum::s_chunk_data_new_line_2:
				if (input == '\n')
				{
					this->state = request_parser::state_enum::s_chunk_size;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			default:
				throw ParseError("unable to parse http request", _ERROR_DETAILS_);
		}
	}

	throw ParseError("unable to parse http request", _ERROR_DETAILS_);
}

// Checks if a byte is an HTTP character.
bool request_parser::is_char(uint c)
{
	return c >= 0 && c <= 127;
}

// Checks if a byte is an HTTP control character.
bool request_parser::is_control(uint c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

// Checks if a byte is defined as an HTTP special character.
bool request_parser::is_special(uint c)
{
	switch (c)
	{
		case '(':
		case ')':
		case '<':
		case '>':
		case '@':
		case ',':
		case ';':
		case ':':
		case '\\':
		case '"':
		case '/':
		case '[':
		case ']':
		case '?':
		case '=':
		case '{':
		case '}':
		case ' ':
		case '\t':
			return true;
		default:
			return false;
	}
}

// Checks if a byte is a digit.
bool request_parser::is_digit(uint c)
{
	return c >= '0' && c <= '9';
}

__INTERNAL_END__
