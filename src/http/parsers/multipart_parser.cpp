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

#include "multipart_parser.h"


__INTERNAL_BEGIN__

// Private methods.
void MultipartParser::_appendParameter(const std::string& key, const std::string& value)
{
	if (!this->_postValues.contains(key))
	{
		this->_postValues.set(key, value);
	}

	this->_multiPostValue.append(key, value);
}

void MultipartParser::_appendFile(
	const std::string& key,
	const std::string& fileName,
	const std::string& contentType,
	const std::string& boundary,
	const std::string& contentDisposition,
	const std::vector<byte>& data
)
{
	bool rootIsEmpty = this->_mediaRoot.empty();
	if (!rootIsEmpty)
	{
		File file(data, this->_mediaRoot + "/" + fileName);
		if (file.isOpen())
		{
			file.close();
		}
	}

	UploadedFile file(this->_mediaRoot + "/" + fileName, rootIsEmpty ? 0 : data.size(), contentType, "", boundary);
	if (!this->_fileValues.contains(key))
	{
		this->_fileValues.set(key, file);
	}

	this->_multiFileValue.append(key, file);
}

// Private static functions.
std::string MultipartParser::_getBoundary(const std::string& contentType)
{
	if (!str::starts_with(contentType, "multipart/"))
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

	str::ltrim(boundary, '-');
	return boundary;
}

void MultipartParser::_assertBoundary(const std::string& actual, const std::string& expected)
{
	if (str::trim(actual, '-') != str::trim(expected, '-'))
	{
		throw MultiPartParserError("Unable to parse request body: invalid boundary", _ERROR_DETAILS_);
	}
}


// Public methods.
MultipartParser::MultipartParser(const std::string& mediaRoot)
{
	this->_mediaRoot = mediaRoot;
	this->_state = MultipartParser::State::BoundaryBegin;
	this->_multiPostValue = MultiValueDict<std::string, std::string>(true);
	this->_postValues = Dict<std::string, std::string>(true);
	this->_multiFileValue = MultiValueDict<std::string, UploadedFile>(true);
	this->_fileValues = Dict<std::string, UploadedFile>(true);
}

void MultipartParser::parse(const std::string& contentType, const std::string& body)
{
	std::string boundary = MultipartParser::_getBoundary(contentType);
	std::string currentBoundary;
	std::string contentDisposition;
	std::string key, value;
	std::vector<byte> file;
	std::string fileContentType;
	std::string fileName;
	size_t endPos = 0, contentIxd = 0;
	auto begin = body.begin();
	auto end = body.end();
	while (begin != end)
	{
		char input = *begin++;
		switch (this->_state)
		{
			case MultipartParser::State::BoundaryBegin:
				if (input == '\r' || input == '\n')
				{
					throw MultiPartParserError("Unable to parse request body: invalid boundary structure", _ERROR_DETAILS_);
				}
				if (input == '-')
				{
					currentBoundary += '-';
				}
				else
				{
					currentBoundary += input;
					this->_state = MultipartParser::State::Boundary;
				}
				break;
			case MultipartParser::State::Boundary:
				if (input == '\r')
				{
					this->_state = MultipartParser::State::BoundaryEnd;
				}
				else if (input == '-')
				{
					this->_state = MultipartParser::State::BodyEnd;
				}
				else
				{
					currentBoundary += input;
				}
				break;
			case MultipartParser::State::BoundaryEnd:
				if (input == '\n')
				{
					this->_assertBoundary(currentBoundary, boundary);
					this->_state = MultipartParser::State::ContentDispositionBegin;
				}
				else
				{
					throw MultiPartParserError("Unable to parse request body: invalid boundary ending", _ERROR_DETAILS_);
				}
				break;
			case MultipartParser::State::BodyEnd:
				if (input == '-')
				{
					this->_assertBoundary(currentBoundary, boundary);
					return;
				}
				break;
			case MultipartParser::State::ContentDispositionBegin:
				if (input == ':')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = MultipartParser::State::ContentDisposition;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case MultipartParser::State::ContentDisposition:
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = MultipartParser::State::NameBegin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				else
				{
					contentDisposition += input;
				}
				break;
			case MultipartParser::State::NameBegin:
				if (input == '=')
				{
					input = *begin++;
					if (input == '"')
					{
						this->_state = MultipartParser::State::Name;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content disposition", _ERROR_DETAILS_);
					}
				}
				break;
			case MultipartParser::State::Name:
				if (input == '"')
				{
					this->_state = MultipartParser::State::NameEnd;
				}
				else
				{
					key += input;
				}
				break;
			case MultipartParser::State::NameEnd:
				if (input == ';')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = MultipartParser::State::FileNameBegin;
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
						this->_state = MultipartParser::State::ContentBegin;
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
			case MultipartParser::State::FileNameBegin:
				fileName.clear();
				if (input == '=')
				{
					input = *begin++;
					if (input == '"')
					{
						this->_state = MultipartParser::State::FileName;
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
			case MultipartParser::State::FileName:
				if (input == '"')
				{
					input = *begin++;
					if (input == '\r')
					{
						input = *begin++;
						if (input == '\n')
						{
							this->_state = MultipartParser::State::ContentTypeBegin;
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
					fileName += input;
				}
				break;
			case MultipartParser::State::ContentTypeBegin:
				if (input == ':')
				{
					input = *begin++;
					if (input == ' ')
					{
						this->_state = MultipartParser::State::ContentType;
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
			case MultipartParser::State::ContentType:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						this->_state = MultipartParser::State::ContentBegin;
					}
					else
					{
						throw MultiPartParserError("Unable to parse content type: missing end of line", _ERROR_DETAILS_);
					}
				}
				else
				{
					fileContentType += input;
				}
				break;
			case MultipartParser::State::ContentBegin:
				if (input == '\r')
				{
					input = *begin++;
					if (input == '\n')
					{
						contentIxd = begin - body.begin();
						endPos = body.find("\r\n" + currentBoundary, contentIxd);
						if (endPos == std::string::npos)
						{
							throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
						}

						this->_state = MultipartParser::State::Content;
					}
					else
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}
				}
				break;
			case MultipartParser::State::Content:
				if (contentIxd + 1 >= endPos)
				{
					if (contentIxd + 1 == endPos)
					{
						input = *begin++;
					}

					if (input != '\r')
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}

					input = *begin++;
					if (input != '\n')
					{
						throw MultiPartParserError("Unable to parse request body: invalid content structure", _ERROR_DETAILS_);
					}

					endPos += 2;

					currentBoundary.clear();
					this->_state = MultipartParser::State::BoundaryBegin;
					if (fileContentType.empty())
					{
						this->_appendParameter(key, value);
						value.clear();
					}
					else
					{
						this->_appendFile(key, fileName, fileContentType, boundary, contentDisposition, file);
						file.clear();
						contentDisposition.clear();
					}

					key.clear();
					fileContentType.clear();
				}
				else
				{
					contentIxd++;
					if (fileContentType.empty())
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

HttpRequest::Parameters<std::string, UploadedFile>* MultipartParser::getFilesParams()
{
	return new HttpRequest::Parameters<std::string, UploadedFile>(this->_fileValues, this->_multiFileValue);
}

HttpRequest::Parameters<std::string, std::string>* MultipartParser::getPostParams()
{
	return new HttpRequest::Parameters<std::string, std::string>(this->_postValues, this->_multiPostValue);
}

__INTERNAL_END__
