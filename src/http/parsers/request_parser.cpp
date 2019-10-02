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
 * http_parser implementation.
 * TODO: write docs.
 */

#include <iostream>

#include "request_parser.h"


__INTERNAL_BEGIN__

// Public methods.
wasp::HttpRequest HttpRequestParser::buildHttpRequest()
{
	return wasp::HttpRequest(
		this->_method,
		this->_path,
		this->_majorV,
		this->_minorV,
		this->_query,
		this->_keepAlive,
		this->_content,
		this->_headers,
		this->_getParameters,
		this->_postParameters
	);
}

Dict<std::string, std::string> HttpRequestParser::getHeaders()
{
	return Dict<std::string, std::string>(this->_headers);
}

void HttpRequestParser::parseHeaders(const std::string& data)
{
	std::string newHeaderName;
	std::string newHeaderValue;

	std::map<std::string, std::string>::iterator connectionIt;

	for (const char& input : data)
	{
		switch (this->_state)
		{
			case ParserState::MethodBegin:
				if (!HttpRequestParser::isChar(input) || HttpRequestParser::isControl(input) || HttpRequestParser::isSpecial(input))
				{
					throw ParseError("unable to parse method type", _ERROR_DETAILS_);
				}
				else
				{
					this->_state = ParserState::Method;
					this->_method.push_back(input);
				}
				break;
			case ParserState::Method:
				if (input == ' ')
				{
					this->_state = PathBegin;
				}
				else if (!HttpRequestParser::isChar(input) || HttpRequestParser::isControl(input) || HttpRequestParser::isSpecial(input))
				{
					throw ParseError("unable to parse http method type", _ERROR_DETAILS_);
				}
				else
				{
					this->_method.push_back(input);
				}
				break;
			case ParserState::PathBegin:
				if (HttpRequestParser::isControl(input))
				{
					throw ParseError("unable to parse http url path", _ERROR_DETAILS_);
				}
				else
				{
					this->_state = Path;
					this->_path.push_back(input);
				}
				break;
			case ParserState::Path:
				if (input == ' ')
				{
					this->_state = ParserState::HttpVersionH;
				}
				else if (input == '?')
				{
					this->_state = ParserState::Query;
				}
				else if (input == '\r')
				{
					this->_majorV = 0;
					this->_minorV = 9;

					return;
				}
				else if (HttpRequestParser::isControl(input))
				{
					throw ParseError("unable to parse http url path", _ERROR_DETAILS_);
				}
				else
				{
					this->_path.push_back(input);
				}
				break;
			case ParserState::Query:
				if (input == ' ')
				{
					this->_state = ParserState::HttpVersionH;
				}
				else if (input == '#')
				{
					this->_state = ParserState::Fragment;
				}
				else if (input == '\r')
				{
					this->_majorV = 0;
					this->_minorV = 9;

					return;
				}
				else if (HttpRequestParser::isControl(input))
				{
					throw ParseError("unable to parse http url query", _ERROR_DETAILS_);
				}
				else
				{
					this->_query.push_back(input);
				}
				break;
			case ParserState::Fragment:
				if (input == ' ')
				{
					this->_state = ParserState::HttpVersionH;
				}
				// INFO: save fragment maybe.
				break;
			case ParserState::HttpVersionH:
				this->_parseHttpWord(input, 'H', ParserState::HttpVersionHt);
				break;
			case ParserState::HttpVersionHt:
				this->_parseHttpWord(input, 'T', ParserState::HttpVersionHtt);
				break;
			case ParserState::HttpVersionHtt:
				this->_parseHttpWord(input, 'T', ParserState::HttpVersionHttp);
				break;
			case ParserState::HttpVersionHttp:
				this->_parseHttpWord(input, 'P', ParserState::HttpVersionSlash);
				break;
			case ParserState::HttpVersionSlash:
				if (input == '/')
				{
					this->_majorV = 0;
					this->_minorV = 0;
					this->_state = ParserState::HttpVersionMajorBegin;
				}
				else
				{
					throw ParseError("unable to parse http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionMajorBegin:
				if (HttpRequestParser::isDigit(input))
				{
					this->_majorV = input - '0';
					this->_state = ParserState::HttpVersionMajor;
				}
				else
				{
					throw ParseError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionMajor:
				if (input == '.')
				{
					this->_state = ParserState::HttpVersionMinorBegin;
				}
				else if (HttpRequestParser::isDigit(input))
				{
					this->_majorV = this->_majorV * 10 + input - '0';
				}
				else
				{
					throw ParseError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionMinorBegin:
				if (HttpRequestParser::isDigit(input))
				{
					this->_minorV = input - '0';
					this->_state = ParserState::HttpVersionMinor;
				}
				else
				{
					throw ParseError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionMinor:
				if (input == '\r')
				{
					this->_state = ParserState::HttpVersionNewLine;
				}
				else if(HttpRequestParser::isDigit(input))
				{
					this->_minorV = this->_minorV * 10 + input - '0';
				}
				else
				{
					throw ParseError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionNewLine:
				if (input == '\n')
				{
					this->_state = ParserState::HeaderLineStart;
				}
				else
				{
					throw ParseError("unable to parse http protocol main data", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HeaderLineStart:
				if (input == '\r')
				{
					this->_state = ParserState::ExpectingNewline_3;
				}
				else if (!this->_headers.empty() && (input == ' ' || input == '\t'))
				{
					this->_state = ParserState::HeaderLws;
				}
				else if(!isChar(input) || isControl(input) || isSpecial(input))
				{
					throw ParseError("unable to parse http request header", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderName.reserve(16);
					newHeaderValue.reserve(16);
					newHeaderName.push_back(input);
					this->_state = ParserState::HeaderName;
				}
				break;
			case ParserState::HeaderLws:
				if(input == '\r')
				{
					this->_state = ParserState::ExpectingNewline_2;
				}
				else if(input == ' ' || input == '\t')
				{
				}
				else if (isControl(input))
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				else
				{
					this->_state = HeaderValue;
					newHeaderValue.push_back(input);
				}
				break;
			case ParserState::HeaderName:
				if (input == ':')
				{
					this->_state = ParserState::HeaderSpaceBeforeValue;
				}
				else if (!isChar(input) || isControl(input) || isSpecial(input))
				{
					throw ParseError("unable to parse http request header name", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderName.push_back(input);
				}
				break;
			case ParserState::HeaderSpaceBeforeValue:
				if (input == ' ')
				{
					this->_state = ParserState::HeaderValue;
				}
				else
				{
					throw ParseError("unable to parse http request header", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HeaderValue:
				if (input == '\r')
				{
					if (strcasecmp(newHeaderName.c_str(), "Content-Length") == 0)
					{
						this->_contentSize = strtol(newHeaderValue.c_str(), nullptr, 0);
						this->_content.reserve(this->_contentSize);
					}
					else if (strcasecmp(newHeaderName.c_str(), "Transfer-Encoding") == 0)
					{
						if (strcasecmp(newHeaderValue.c_str(), "chunked") == 0)
						{
							this->_chunked = true;
						}
					}

					this->_headers[newHeaderName] = newHeaderValue;
					newHeaderName.clear();
					newHeaderValue.clear();
					this->_state = ParserState::ExpectingNewline_2;
				}
				else if (HttpRequestParser::isControl(input))
				{
					throw ParseError("unable to parse http request header value", _ERROR_DETAILS_);
				}
				else
				{
					newHeaderValue.push_back(input);
				}
				break;
			case ParserState::ExpectingNewline_2:
				if (input == '\n')
				{
					this->_state = ParserState::HeaderLineStart;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ExpectingNewline_3:
				connectionIt = std::find_if(
					this->_headers.begin(),
					this->_headers.end(),
					[](const std::pair<std::string, std::string>& item) -> bool
					{
						return strcasecmp(item.first.c_str(), "Connection") == 0;
					}
				);
				if (connectionIt != this->_headers.end())
				{
					this->_keepAlive = strcasecmp((*connectionIt).second.c_str(), "Keep-Alive") == 0;
				}
				else
				{
					if (this->_majorV > 1 || (this->_majorV == 1 && this->_minorV == 1))
					{
						this->_keepAlive = true;
					}
				}

				if (this->_chunked)
				{
					this->_state = ParserState::ChunkSize;
				}
				else if (this->_contentSize == 0)
				{
					if (input != '\n')
					{
						throw ParseError("unable to parse http request", _ERROR_DETAILS_);
					}
				}
				else
				{
					auto contentType = this->_headers["Content-Type"];
					if (contentType.find("multipart/form-data") != std::string::npos)
					{
						this->_contentType = ContentType::MultipartFormData;
					}
					else if (contentType.find("application/json") != std::string::npos)
					{
						this->_contentType = ContentType::ApplicationJson;
					}
					else if (contentType.find("application/x-www-form-urlencoded") != std::string::npos)
					{
						this->_contentType = ContentType::ApplicationXWwwFormUrlencoded;
					}
					else
					{
						this->_contentType = ContentType::Other;
					}
					this->_state = ParserState::RequestBody;
				}
				return;
			default:
				throw ParseError("unable to parse http request", _ERROR_DETAILS_);
		}
	}

	throw ParseError("unable to parse http request", _ERROR_DETAILS_);
}

void HttpRequestParser::parseBody(const std::string& data)
{
	QueryParser queryParser;
	MultipartParser multipartParser;
	if (data.empty())
	{
		this->_setParameters(queryParser.parse(this->_query));
	}
	else
	{
		this->_parseBody(data);
		if (!this->_content.empty())
		{
			switch (this->_contentType)
			{
				case ContentType::ApplicationXWwwFormUrlencoded:
					this->_setParameters(queryParser.parse(this->_content));
					break;
				case ContentType::ApplicationJson:
					// TODO: parse application/json data
					break;
				case ContentType::MultipartFormData:
					multipartParser.parse(this->_headers["Content-Type"], this->_content);
					break;
				case ContentType::Other:
					break;
				default:
					break;
			}
		}
	}
}

void HttpRequestParser::reset()
{
	this->_majorV = 0;
	this->_minorV = 0;
	this->_path.clear();
	this->_query.clear();
	this->_method.clear();
	this->_keepAlive = false;
	this->_content.clear();
	this->_headers.clear();
	this->_getParameters = RequestParameters<std::string, std::string>();
	this->_postParameters = RequestParameters<std::string, std::string>();
}

// Private methods.
void HttpRequestParser::_parseHttpWord(char input, char expectedInput, HttpRequestParser::ParserState newState)
{
	if (input == expectedInput)
	{
		this->_state = newState;
	}
	else
	{
		throw ParseError("unable to parse http protocol version", _ERROR_DETAILS_);
	}
}

void HttpRequestParser::setParameters(RequestParameters<std::string, std::string>* params)
{
	if (this->_method == "GET")
	{
		this->_getParameters = *params;
	}
	else if (this->_method == "POST")
	{
		this->_postParameters = *params;
	}
}

void HttpRequestParser::_setParameters(RequestParameters<std::string, std::string>* params)
{
	this->setParameters(params);
	delete params;
}

void HttpRequestParser::_parseBody(const std::string& data)
{
	if (this->_state == ParserState::RequestBody)
	{
		// size of '\r\n'
		unsigned endingSize = 2;
		this->_content.append(data.substr(0, data.size() - endingSize));
		if (this->_content.size() == this->_contentSize - endingSize)
		{
			return;
		}
	}

	for (const char& input : data)
	{
		switch (this->_state)
		{
			case ParserState::ChunkSize:
				if (isalnum(input))
				{
					this->_chunkSizeStr.push_back(input);
				}
				else if (input == ';')
				{
					this->_state = ParserState::ChunkExtensionName;
				}
				else if (input == '\r')
				{
					this->_state = ParserState::ChunkSizeNewLine;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkExtensionName:
				if (isalnum(input) || input == ' ')
				{
					// skip
				}
				else if (input == '=')
				{
					this->_state = ParserState::ChunkExtensionValue;
				}
				else if (input == '\r')
				{
					this->_state = ParserState::ChunkSizeNewLine;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkExtensionValue:
				if (isalnum(input) || input == ' ')
				{
					// skip
				}
				else if (input == '\r')
				{
					this->_state = ParserState::ChunkSizeNewLine;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkSizeNewLine:
				if (input == '\n')
				{
					this->_chunkSize = strtol(this->_chunkSizeStr.c_str(), nullptr, 16);
					this->_chunkSizeStr.clear();
					this->_content.reserve(this->_content.size() + this->_chunkSize);

					if (this->_chunkSize == 0)
					{
						this->_state = ParserState::ChunkSizeNewLine_2;
					}
					else
					{
						this->_state = ParserState::ChunkData;
					}
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkSizeNewLine_2:
				if (input == '\r')
				{
					this->_state = ParserState::ChunkSizeNewLine_3;
				}
				else if( isalpha(input) )
				{
					this->_state = ParserState::ChunkTrailerName;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkSizeNewLine_3:
				if (input == '\n')
				{
					return;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
			case ParserState::ChunkTrailerName:
				if (isalnum(input))
				{
					// skip
				}
				else if (input == ':')
				{
					this->_state = ParserState::ChunkTrailerValue;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkTrailerValue:
				if (isalnum(input) || input == ' ')
				{
					// skip
				}
				else if( input == '\r' )
				{
					this->_state = ParserState::ChunkSizeNewLine;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkData:
				this->_content.push_back(input);

				if (--this->_chunkSize == 0)
				{
					this->_state = ParserState::ChunkDataNewLine_1;
				}
				break;
			case ParserState::ChunkDataNewLine_1:
				if (input == '\r')
				{
					this->_state = ParserState::ChunkDataNewLine_2;
				}
				else
				{
					throw ParseError("unable to parse http request", _ERROR_DETAILS_);
				}
				break;
			case ParserState::ChunkDataNewLine_2:
				if (input == '\n')
				{
					this->_state = ParserState::ChunkSize;
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

// Private static functions.
bool HttpRequestParser::isChar(uint c)
{
	return c >= 0 && c <= 127;
}

bool HttpRequestParser::isControl(uint c)
{
	return (c >= 0 && c <= 31) || (c == 127);
}

bool HttpRequestParser::isSpecial(uint c)
{
	switch (c)
	{
		case '(': case ')': case '<': case '>': case '@':
		case ',': case ';': case ':': case '\\': case '"':
		case '/': case '[': case ']': case '?': case '=':
		case '{': case '}': case ' ': case '\t':
			return true;
		default:
			return false;
	}
}

bool HttpRequestParser::isDigit(uint c)
{
	return c >= '0' && c <= '9';
}

__INTERNAL_END__
