/**
 * http/parsers/multipart_parser.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./multipart_parser.h"

// TODO: remove!
//// Base libraries.
//#include <xalwart.base/exceptions.h>
//#include <xalwart.base/string_utils.h>
//
//
//__HTTP_INTERNAL_BEGIN__
//
//void MultipartParser::append_parameter(const std::string& key, const std::string& value)
//{
//	if (!this->post_values.contains(key))
//	{
//		this->post_values.set(key, value);
//	}
//
//	this->multi_post_value.add(key, value);
//}
//
//void MultipartParser::append_file(
//	const std::string& key,
//	const std::string& file_name,
//	const std::string& content_type,
//	const std::string& boundary,
//	const std::string& content_disposition,
//	std::vector<unsigned char>& data
//)
//{
//	files::UploadedFile uploaded_file(
//		this->media_root + "/" + file_name,
//		this->media_root.empty() ? 0 : data.size(),
//		data,
//		content_type,
//		"",
//		boundary,
//		content_disposition
//	);
//
//	if (!this->file_values.contains(key))
//	{
//		this->file_values.set(key, uploaded_file);
//	}
//
//	this->multi_file_value.add(key, uploaded_file);
//}
//
//std::string MultipartParser::get_boundary(const std::string& content_type)
//{
//	if (!content_type.starts_with("multipart/"))
//	{
//		throw MultiPartParserError("invalid Content-Type: " + content_type, _ERROR_DETAILS_);
//	}
//
//	std::string boundary;
//	size_t pos = content_type.find("boundary=");
//	if (pos != std::string::npos)
//	{
//		boundary = content_type.substr(pos + 9);
//	}
//
//	if (boundary.empty())
//	{
//		throw MultiPartParserError(
//			"unable to parse request body: boundary is empty", _ERROR_DETAILS_
//		);
//	}
//
//	return str::ltrim(boundary, '-');
//}
//
//void MultipartParser::assert_boundary(const std::string& actual, const std::string& expected)
//{
//	if (str::trim(actual, '-') != str::trim(expected, '-'))
//	{
//		throw MultiPartParserError(
//			"unable to parse request body: invalid boundary", _ERROR_DETAILS_
//		);
//	}
//}
//
//MultipartParser::MultipartParser(const std::string& media_root)
//{
//	this->media_root = str::rtrim(media_root, '/');
//}
//
//void MultipartParser::parse(const std::string& content_type, const std::string& body)
//{
//	std::string boundary = MultipartParser::get_boundary(content_type);
//	std::string current_boundary, content_disposition, key, value, file_content_type, file_name;
//	std::vector<unsigned char> file;
//	size_t end_pos = 0, content_idx = 0;
//	auto begin = body.begin();
//	auto end = body.end();
//	MultipartParser::State st = MultipartParser::State::BoundaryBegin;
//	while (begin != end)
//	{
//		char input = *begin++;
//		switch (st)
//		{
//			case MultipartParser::State::BoundaryBegin:
//				if (input == '\r' || input == '\n')
//				{
//					throw MultiPartParserError(
//						"unable to parse request body: invalid boundary structure",
//						_ERROR_DETAILS_
//					);
//				}
//				if (input == '-')
//				{
//					current_boundary += '-';
//				}
//				else
//				{
//					current_boundary += input;
//					st = MultipartParser::State::Boundary;
//				}
//				break;
//			case MultipartParser::State::Boundary:
//				if (input == '\r')
//				{
//					st = MultipartParser::State::BoundaryEnd;
//				}
//				else if (input == '-')
//				{
//					st = MultipartParser::State::BodyEnd;
//				}
//				else
//				{
//					current_boundary += input;
//				}
//				break;
//			case MultipartParser::State::BoundaryEnd:
//				if (input == '\n')
//				{
//					this->assert_boundary(current_boundary, boundary);
//					st = MultipartParser::State::ContentDispositionBegin;
//				}
//				else
//				{
//					throw MultiPartParserError(
//						"unable to parse request body: invalid boundary ending",
//						_ERROR_DETAILS_
//					);
//				}
//				break;
//			case MultipartParser::State::BodyEnd:
//				if (input == '-')
//				{
//					this->assert_boundary(current_boundary, boundary);
//					return;
//				}
//				break;
//			case MultipartParser::State::ContentDispositionBegin:
//				if (input == ':')
//				{
//					input = *begin++;
//					if (input == ' ')
//					{
//						st = MultipartParser::State::ContentDisposition;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content disposition",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				break;
//			case MultipartParser::State::ContentDisposition:
//				if (input == ';')
//				{
//					input = *begin++;
//					if (input == ' ')
//					{
//						st = MultipartParser::State::NameBegin;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content disposition",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else
//				{
//					content_disposition += input;
//				}
//				break;
//			case MultipartParser::State::NameBegin:
//				if (input == '=')
//				{
//					input = *begin++;
//					if (input == '"')
//					{
//						st = MultipartParser::State::Name;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content disposition",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				break;
//			case MultipartParser::State::Name:
//				if (input == '"')
//				{
//					st = MultipartParser::State::NameEnd;
//				}
//				else
//				{
//					key += input;
//				}
//				break;
//			case MultipartParser::State::NameEnd:
//				if (input == ';')
//				{
//					input = *begin++;
//					if (input == ' ')
//					{
//						st = MultipartParser::State::FileNameBegin;
//						file_name.clear();
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content disposition",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else if (input == '\r')
//				{
//					input = *begin++;
//					if (input == '\n')
//					{
//						st = MultipartParser::State::ContentBegin;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content disposition",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else
//				{
//					throw MultiPartParserError(
//						"unable to parse request body: invalid content disposition",
//						_ERROR_DETAILS_
//					);
//				}
//				break;
//			case MultipartParser::State::FileNameBegin:
//				if (input == '=')
//				{
//					input = *begin++;
//					if (input == '"')
//					{
//						st = MultipartParser::State::FileName;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse filename: missing \" before filename",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else if (input == '\r')
//				{
//					throw MultiPartParserError(
//						"unable to parse filename: invalid filename structure",
//						_ERROR_DETAILS_
//					);
//				}
//				break;
//			case MultipartParser::State::FileName:
//				if (input == '"')
//				{
//					input = *begin++;
//					if (input == '\r')
//					{
//						input = *begin++;
//						if (input == '\n')
//						{
//							st = MultipartParser::State::ContentTypeBegin;
//						}
//						else
//						{
//							throw MultiPartParserError(
//								"unable to parse filename: missing end of line",
//								_ERROR_DETAILS_
//							);
//						}
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse filename: control symbol \\r",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else
//				{
//					file_name += input;
//				}
//				break;
//			case MultipartParser::State::ContentTypeBegin:
//				if (input == ':')
//				{
//					input = *begin++;
//					if (input == ' ')
//					{
//						st = MultipartParser::State::ContentType;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse content type: invalid content type structure",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else if (input == '\r')
//				{
//					throw MultiPartParserError(
//						"unable to parse content type: invalid content type structure",
//						_ERROR_DETAILS_
//					);
//				}
//				break;
//			case MultipartParser::State::ContentType:
//				if (input == '\r')
//				{
//					input = *begin++;
//					if (input == '\n')
//					{
//						st = MultipartParser::State::ContentBegin;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse content type: missing end of line",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				else
//				{
//					file_content_type += input;
//				}
//				break;
//			case MultipartParser::State::ContentBegin:
//				if (input == '\r')
//				{
//					input = *begin++;
//					if (input == '\n')
//					{
//						content_idx = begin - body.begin();
//						end_pos = body.find("\r\n" + current_boundary, content_idx);
//						if (end_pos == std::string::npos)
//						{
//							throw MultiPartParserError(
//								"unable to parse request body: invalid content structure",
//								_ERROR_DETAILS_
//							);
//						}
//
//						st = MultipartParser::State::Content;
//					}
//					else
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content structure",
//							_ERROR_DETAILS_
//						);
//					}
//				}
//				break;
//			case MultipartParser::State::Content:
//				if (content_idx + 1 > end_pos)
//				{
//					if (input != '\r')
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content structure",
//							_ERROR_DETAILS_
//						);
//					}
//
//					input = *begin++;
//					if (input != '\n')
//					{
//						throw MultiPartParserError(
//							"unable to parse request body: invalid content structure",
//							_ERROR_DETAILS_
//						);
//					}
//
//					end_pos += 2;
//					current_boundary.clear();
//					st = MultipartParser::State::BoundaryBegin;
//					if (file_content_type.empty())
//					{
//						this->append_parameter(key, value);
//						value.clear();
//					}
//					else
//					{
//						if (!file.empty())
//						{
//							this->append_file(
//								key, file_name, file_content_type, boundary, content_disposition, file
//							);
//							file.clear();
//						}
//
//						content_disposition.clear();
//					}
//
//					key.clear();
//					file_content_type.clear();
//				}
//				else
//				{
//					content_idx++;
//					if (file_content_type.empty())
//					{
//						value += input;
//					}
//					else
//					{
//						file.push_back(input);
//					}
//				}
//				break;
//			default:
//				throw MultiPartParserError("unable to parse request body", _ERROR_DETAILS_);
//		}
//	}
//
//	throw MultiPartParserError("unable to parse request body", _ERROR_DETAILS_);
//}
//
//__HTTP_INTERNAL_END__
