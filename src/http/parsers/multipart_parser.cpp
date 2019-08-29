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
 * multipart_parser implementation.
 * TODO: write docs.
 */

#include "multipart_parser.h"


__INTERNAL_BEGIN__

// Private methods.
void MultipartParser::_appendParameter(const std::string& key, const std::string& value)
{
	if (!this->_dict.contains(key))
	{
		this->_dict.set(key, value);
	}
	this->_multiDict.append(key, value);
}

// Private static functions.
std::string MultipartParser::_getBoundary(const std::string& contentType)
{
	if (!str::startsWith(contentType, "multipart/"))
	{
		throw MultiPartParserError("Invalid Content-Type: " + contentType, _ERROR_DETAILS_);
	}

	std::string boundary;
	size_t pos = contentType.find("boundary=");
	if (pos != std::string::npos)
	{
		boundary = contentType.substr(pos + 9);
	}

	if (boundary.empty())
	{
		throw MultiPartParserError("Unable to parse request body: boundary is empty", _ERROR_DETAILS_);
	}

	return boundary;
}

void MultipartParser::_assertBoundary(const std::string& actual, const std::string& expected)
{
	if (actual != expected)
	{
		throw MultiPartParserError("Unable to parse request body: invalid boundary", _ERROR_DETAILS_);
	}
}


// Public methods.
MultipartParser::MultipartParser(const UploadHandler& uploadHandler)
{
	this->_uploadHandler = uploadHandler;
	this->_state = ParserState::BoundaryBegin;
}

void MultipartParser::parse(const std::string& contentType, const std::string& body)
{
	std::string boundary = MultipartParser::_getBoundary(contentType);
	std::string currentBoundary;
	std::string key, value;
	std::string fileContentType;
	auto begin = body.begin();
	auto end = body.end();
	while (begin != end)
	{
		char input = *begin++;
		switch (this->_state)
		{
			case ParserState::BoundaryBegin:
				currentBoundary.clear();
				if (input != '-')
				{
					this->_state = ParserState::Boundary;
				}
				else
				{
					currentBoundary += '-';
				}
				break;
			case ParserState::Boundary:
				if (input == '\r')
				{
					input = *begin++;
					this->_state = ParserState::BoundaryEnd;
				}
				else if (input == '-')
				{
					this->_state = ParserState::BodyEnd;
				}
				else
				{
					currentBoundary += input;
				}
				break;
			case ParserState::BoundaryEnd:
				if (input == '\n')
				{
					this->_assertBoundary(currentBoundary, boundary);
					this->_state = ParserState::ContentDispositionBegin;
				}
				else
				{
					throw MultiPartParserError("Unable to parse request body: invalid boundary ending", _ERROR_DETAILS_);
				}
				break;
			case ParserState::BodyEnd:
				if (input == '-')
				{
					this->_assertBoundary(currentBoundary, boundary);
					return;
				}
				break;
			case ParserState::ContentDispositionBegin:
				if (input == ':')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = ParserState::ContentDisposition;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case ParserState::ContentDisposition:
				// Save content-disposition type maybe
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = ParserState::NameBegin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case ParserState::NameBegin:
				if (input == '=')
				{
					input = *begin++;
					if (input == '"')
					{
						this->_state = ParserState::Name;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case ParserState::Name:
				if (input == '"')
				{
					this->_state = ParserState::NameEnd;
				}
				else
				{
					key += input;
				}
				break;
			case ParserState::NameEnd:
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = ParserState::FileNameBegin;
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
						this->_state = ParserState::ContentBegin;
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
			case ParserState::FileNameBegin:
				break;
			case ParserState::FileName:
				break;
			case ParserState::ContentTypeBegin:
				break;
			case ParserState::ContentType:
				break;
			case ParserState::ContentBegin:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						this->_state = ParserState::Content;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}
				}
				break;
			case ParserState::Content:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						this->_state = ParserState::BoundaryBegin;
						if (fileContentType.empty())
						{
							this->_appendParameter(key, value);
							value.clear();
						}
						else
						{
							// TODO: append file
						}
						key.clear();
						fileContentType.clear();
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}
				}
				else
				{
					if (fileContentType.empty())
					{
						value += input;
					}
					else
					{
						// TODO: append to vector<char>
					}
				}
				break;
			default:
				throw MultiPartParserError("Unable to parse request body", _ERROR_DETAILS_);
		}
	}

	throw MultiPartParserError("Unable to parse request body", _ERROR_DETAILS_);
}

MultiValueDict<std::string, std::string> MultipartParser::getFilesParameters()
{
	return MultiValueDict<std::string, std::string>();
}

Dict<std::string, std::string> MultipartParser::getPostParameters()
{
	return Dict<std::string, std::string>();
}

__INTERNAL_END__
