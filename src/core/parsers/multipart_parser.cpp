/**
 * core/parsers/multipart_parser.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./multipart_parser.h"

// Core libraries.
#include <xalwart.core/exceptions.h>
#include <xalwart.core/string_utils.h>


__CORE_INTERNAL_BEGIN__

void multipart_parser::append_parameter(const std::string& key, const std::string& value)
{
	if (!this->post_values.contains(key))
	{
		this->post_values.set(key, value);
	}

	this->multi_post_value.append(key, value);
}

void multipart_parser::append_file(
	const std::string& key,
	const std::string& file_name,
	const std::string& content_type,
	const std::string& boundary,
	const std::string& content_disposition,
	const std::vector<byte>& data
)
{
	bool root_is_empty = this->media_root.empty();
	File file;
	if (!root_is_empty)
	{
		file = File(data, this->media_root + "/" + file_name);
	}

	UploadedFile uploaded_file(
		this->media_root + "/" + file_name,
		root_is_empty ? 0 : data.size(),
		file,
		content_type,
		"",
		boundary,
		content_disposition
	);

	if (!this->file_values.contains(key))
	{
		this->file_values.set(key, uploaded_file);
	}

	this->multi_file_value.append(key, uploaded_file);
}

std::string multipart_parser::get_boundary(const std::string& content_type)
{
	if (!str::starts_with(content_type, "multipart/"))
	{
		throw MultiPartParserError("Invalid Content-Type: " + content_type, _ERROR_DETAILS_);
	}

	std::string boundary;
	size_t pos = content_type.find("boundary=");
	if (pos != std::string::npos)
	{
		boundary = content_type.substr(pos + 9);
	}

	if (boundary.empty())
	{
		throw MultiPartParserError("Unable to parse request body: boundary is empty", _ERROR_DETAILS_);
	}

	str::ltrim(boundary, "-");
	return boundary;
}

void multipart_parser::assert_boundary(const std::string& actual, const std::string& expected)
{
	if (str::trim(actual, "-") != str::trim(expected, "-"))
	{
		throw MultiPartParserError("Unable to parse request body: invalid boundary", _ERROR_DETAILS_);
	}
}

multipart_parser::multipart_parser(const std::string& media_root)
{
	this->media_root = str::rtrim(media_root, "/");
}

void multipart_parser::parse(const std::string& content_type, const std::string& body)
{
	std::string boundary = multipart_parser::get_boundary(content_type);
	std::string current_boundary, content_disposition, key, value, file_content_type, file_name;
	std::vector<byte> file;
	size_t end_pos = 0, content_idx = 0;
	auto begin = body.begin();
	auto end = body.end();
	multipart_parser::state st = multipart_parser::state::s_boundary_begin;
	while (begin != end)
	{
		char input = *begin++;
		switch (st)
		{
			case multipart_parser::state::s_boundary_begin:
				if (input == '\r' || input == '\n')
				{
					throw MultiPartParserError("Unable to parse request body: invalid boundary structure", _ERROR_DETAILS_);
				}
				if (input == '-')
				{
					current_boundary += '-';
				}
				else
				{
					current_boundary += input;
					st = multipart_parser::state::s_boundary;
				}
				break;
			case multipart_parser::state::s_boundary:
				if (input == '\r')
				{
					st = multipart_parser::state::s_boundary_end;
				}
				else if (input == '-')
				{
					st = multipart_parser::state::s_body_end;
				}
				else
				{
					current_boundary += input;
				}
				break;
			case multipart_parser::state::s_boundary_end:
				if (input == '\n')
				{
					this->assert_boundary(current_boundary, boundary);
					st = multipart_parser::state::s_content_disposition_begin;
				}
				else
				{
					throw MultiPartParserError("Unable to parse request body: invalid boundary ending", _ERROR_DETAILS_);
				}
				break;
			case multipart_parser::state::s_body_end:
				if (input == '-')
				{
					this->assert_boundary(current_boundary, boundary);
					return;
				}
				break;
			case multipart_parser::state::s_content_disposition_begin:
				if (input == ':')
				{
					input = *begin++;
					if (input == ' ')
					{
						st = multipart_parser::state::s_content_disposition;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case multipart_parser::state::s_content_disposition:
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						st = multipart_parser::state::s_name_begin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				else
				{
					content_disposition += input;
				}
				break;
			case multipart_parser::state::s_name_begin:
				if (input == '=')
				{
					input = *begin++;
					if (input == '"')
					{
						st = multipart_parser::state::s_name;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case multipart_parser::state::s_name:
				if (input == '"')
				{
					st = multipart_parser::state::s_name_end;
				}
				else
				{
					key += input;
				}
				break;
			case multipart_parser::state::s_name_end:
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						st = multipart_parser::state::s_file_name_begin;
						file_name.clear();
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				else if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						st = multipart_parser::state::s_content_begin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				else
				{
					throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
				}
				break;
			case multipart_parser::state::s_file_name_begin:
				if (input == '=')
				{
					input = *begin++;
					if (input == '"')
					{
						st = multipart_parser::state::s_file_name;
					}
					else
					{
						throw MultiPartParserError("Unable to parse filename: missing \" before filename", _ERROR_DETAILS_);
					}
				}
				else if (input == '\r')
				{
					throw MultiPartParserError("Unable to parse filename: invalid filename structure", _ERROR_DETAILS_);
				}
				break;
			case multipart_parser::state::s_file_name:
				if (input == '"')
				{
					input = *begin++;
					if (input == '\r')
					{
						input = *begin++;
						if (input == '\n')
						{
							st = multipart_parser::state::s_content_type_begin;
						}
						else
						{
							throw MultiPartParserError("Unable to parse filename: missing end of line", _ERROR_DETAILS_);
						}
					}
					else
					{
						throw MultiPartParserError("Unable to parse filename: control symbol \\r", _ERROR_DETAILS_);
					}
				}
				else
				{
					file_name += input;
				}
				break;
			case multipart_parser::state::s_content_type_begin:
				if (input == ':')
				{
					input = *begin++;
					if (input == ' ')
					{
						st = multipart_parser::state::s_content_type;
					}
					else
					{
						throw MultiPartParserError("Unable to parse content type: invalid content type structure", _ERROR_DETAILS_);
					}
				}
				else if (input == '\r')
				{
					throw MultiPartParserError("Unable to parse content type: invalid content type structure", _ERROR_DETAILS_);
				}
				break;
			case multipart_parser::state::s_content_type:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						st = multipart_parser::state::s_content_begin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse content type: missing end of line", _ERROR_DETAILS_);
					}
				}
				else
				{
					file_content_type += input;
				}
				break;
			case multipart_parser::state::s_content_begin:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						content_idx = begin - body.begin();
						end_pos = body.find("\r\n" + current_boundary, content_idx);
						if (end_pos == std::string::npos)
						{
							throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
						}

						st = multipart_parser::state::s_content;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}
				}
				break;
			case multipart_parser::state::s_content:
				if (content_idx + 1 > end_pos)
				{
					if (input != '\r')
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}

					input = *begin++;
					if (input != '\n')
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}

					end_pos += 2;
					current_boundary.clear();
					st = multipart_parser::state::s_boundary_begin;
					if (file_content_type.empty())
					{
						this->append_parameter(key, value);
						value.clear();
					}
					else
					{
						if (!file.empty())
						{
							this->append_file(key, file_name, file_content_type, boundary, content_disposition, file);
							file.clear();
						}

						content_disposition.clear();
					}

					key.clear();
					file_content_type.clear();
				}
				else
				{
					content_idx++;
					if (file_content_type.empty())
					{
						value += input;
					}
					else
					{
						file.push_back(input);
					}
				}
				break;
			default:
				throw MultiPartParserError("Unable to parse request body", _ERROR_DETAILS_);
		}
	}

	throw MultiPartParserError("Unable to parse request body", _ERROR_DETAILS_);
}

__CORE_INTERNAL_END__
