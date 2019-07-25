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

#include "request_parser.h"
#include "../../core/exceptions.h"


__INTERNAL_BEGIN__

wasp::HttpRequest HttpRequestParser::parse(const std::string& data)
{
	this->_parseRequest(data);
	if (!this->_content.empty())
	{
		switch (this->_contentType)
		{
			case ContentType::ApplicationXWwwFormUrlencoded:
				this->_setParameters(HttpRequestParser::_parseQuery(this->_content));
				break;
			case ContentType::ApplicationJson:
				this->_parseApplicationJson();
				break;
			case ContentType::MultipartFormData:
				this->_parseMultipart();
				break;
			case ContentType::TextPlain:
				this->_parsePlainText();
				break;
		}
	}
	else
	{
		this->_setParameters(HttpRequestParser::_parseQuery(this->_query));
	}

	// TODO: parse cookies

	return this->_buildHttpRequest();
}

void HttpRequestParser::_parseHttpWord(char input, char expectedInput, HttpRequestParser::ParserState newState)
{
	if (input == expectedInput)
	{
		this->_state = newState;
	}
	else
	{
		throw wasp::HttpError("unable to parse http protocol version", _ERROR_DETAILS_);
	}
}

void HttpRequestParser::_parseRequest(const std::string& data)
{
	auto begin = data.begin();
	auto end = data.end();

	std::string newHeaderName;
	std::string newHeaderValue;

	std::map<std::string, std::string>::iterator connectionIt;

	while (begin != end)
	{
		char input = *begin++;
		switch (this->_state)
		{
			case ParserState::MethodBegin:
				if (!HttpRequestParser::isChar(input) || HttpRequestParser::isControl(input) || HttpRequestParser::isSpecial(input))
				{
					throw wasp::HttpError("unable to parse method type", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse http method type", _ERROR_DETAILS_);
				}
				else
				{
					this->_method.push_back(input);
				}
				break;
			case ParserState::PathBegin:
				if (HttpRequestParser::isControl(input))
				{
					throw wasp::HttpError("unable to parse http url path", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse http url path", __LINE__, __FUNCTION__, __FILE__);
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
				else if (input == '\r')
				{
					this->_majorV = 0;
					this->_minorV = 9;

					return;
				}
				else if (HttpRequestParser::isControl(input))
				{
					throw wasp::HttpError("unable to parse http url query", __LINE__, __FUNCTION__, __FILE__);
				}
				else
				{
					this->_query.push_back(input);
				}
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
					throw wasp::HttpError("unable to parse http protocol version", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse major part of http protocol version", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse minor part of http protocol version", _ERROR_DETAILS_);
				}
				break;
			case ParserState::HttpVersionNewLine:
				if (input == '\n')
				{
					this->_state = ParserState::HeaderLineStart;
				}
				else
				{
					throw wasp::HttpError("unable to parse http protocol main data", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse http request header", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request header name", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request header", _ERROR_DETAILS_);
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
					throw wasp::HttpError("unable to parse http request header value", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
				}
				break;
			case ParserState::ExpectingNewline_3:
				connectionIt = std::find_if(
					this->_headers.begin(),
					this->_headers.end(),
					[](std::pair<std::string, std::string> item) -> bool
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
					if (input == '\n')
					{
						return;
					}
					else
					{
						throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					else if (contentType.find("text/plain") != std::string::npos)
					{
						this->_contentType = ContentType::TextPlain;
					}
					else
					{
						this->_contentType = ContentType::ApplicationXWwwFormUrlencoded;
					}
					this->_state = ParserState::RequestBody;
				}
				break;
			case ParserState::RequestBody:
				--this->_contentSize;
				this->_content.push_back(input);

				if (this->_contentSize == 0)
				{
					return;
				}
				break;
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
				}
				break;
			case ParserState::ChunkSizeNewLine_3:
				if (input == '\n')
				{
					return;
				}
				else
				{
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
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
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
				}
				break;
			case ParserState::ChunkDataNewLine_2:
				if (input == '\n')
				{
					this->_state = ParserState::ChunkSize;
				}
				else
				{
					throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
				}
				break;
			default:
				throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
		}
	}

	throw wasp::HttpError("unable to parse http request", __LINE__, __FUNCTION__, __FILE__);
}

void HttpRequestParser::_parseApplicationJson()
{
	// TODO
}

void HttpRequestParser::_parseMultipart()
{
	auto contentType = this->_headers["Content-Type"];
	auto pos = contentType.find("boundary=");
	if (pos == std::string::npos)
	{
		return;
	}
	auto boundary = contentType.substr(pos + 9);

	// TODO: parse multipart data
}

void HttpRequestParser::_parsePlainText()
{
	// TODO
}

std::map<std::string, std::string>* HttpRequestParser::_parseQuery(const std::string& content)
{
	auto* result = new std::map<std::string, std::string>();
	if (content.empty())
	{
		return result;
	}

	auto begin = content.begin();
	auto end = content.end();
	std::string itemKey;
	std::string itemValue;

	QueryParserState state = QueryParserState::Key;

	while (begin != end)
	{
		char input = *begin++;
		switch (state)
		{
			case QueryParserState::Key:
				if (input == '=')
				{
					state = QueryParserState::Val;
				}
				else
				{
					itemKey.push_back(input);
				}
				break;
			case QueryParserState::Val:
				if (input == '&')
				{
					(*result)[itemKey] = itemValue;
					itemKey.clear();
					itemValue.clear();
					state = QueryParserState::Key;
				}
				else
				{
					itemValue.push_back(input);
				}
				break;
		}
	}
	(*result)[itemKey] = itemValue;

	return result;
}

void HttpRequestParser::setParameters(std::map<std::string, std::string>* params)
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

void HttpRequestParser::_setParameters(std::map<std::string, std::string>* params)
{
	this->setParameters(params);
	delete params;
}

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

wasp::HttpRequest HttpRequestParser::_buildHttpRequest()
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

__INTERNAL_END__
