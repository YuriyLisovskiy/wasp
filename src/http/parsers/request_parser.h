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
#include <string>
#include <cstdlib>
#include <strings.h>
#include <map>

#include "../../globals.h"
#include "../request.h"


__INTERNAL_BEGIN__

class HttpRequestParser
{
private:
	size_t _majorV{};
	size_t _minorV{};
	std::string _path;
	std::string _query;
	std::string _method;
	bool _keepAlive{};
	std::string _content;
	std::map<std::string, std::string> _headers;

	unsigned long long _contentSize{};
	std::string _chunkSizeStr;
	unsigned long long _chunkSize{};
	bool _chunked{};

	wasp::HttpRequest buildHttpRequest();

	// Check if a byte is an HTTP character.
	static inline bool isChar(uint c);

	// Check if a byte is an HTTP control character.
	static inline bool isControl(uint c);

	// Check if a byte is defined as an HTTP special character.
	static inline bool isSpecial(uint c);

	// Check if a byte is a digit.
	static inline bool isDigit(uint c);

	enum ParserState
	{
		MethodBegin,
		Method,
		PathBegin,
		Path,
		HttpVersionH,
		HttpVersionHt,
		HttpVersionHtt,
		HttpVersionHttp,
		HttpVersionSlash,
		HttpVersionMajorBegin,
		HttpVersionMajor,
		HttpVersionMinorBegin,
		HttpVersionMinor,
		HttpVersionNewLine,

		HeaderLineStart,
		HeaderLws,
		HeaderName,
		HeaderSpaceBeforeValue,
		HeaderValue,

		ExpectingNewline_2,
		ExpectingNewline_3,

		Post,
		ChunkSize,
		ChunkExtensionName,
		ChunkExtensionValue,
		ChunkSizeNewLine,
		ChunkSizeNewLine_2,
		ChunkSizeNewLine_3,
		ChunkTrailerName,
		ChunkTrailerValue,

		ChunkDataNewLine_1,
		ChunkDataNewLine_2,
		ChunkData

	} _state{};

	void _parseMethodBegin(char input);
	void _parseMethod(char input);
	void _parsePathBegin(char input);
	void _parseHttpWord(char input, char expectedInput, HttpRequestParser::ParserState newState);
	void _parseVersionSlash(char input);
	void _parseVersionMajorBegin(char input);
	void _parseVersionMajor(char input);

	wasp::HttpRequest _parse(const std::string& data);

public:
	HttpRequestParser();
	wasp::HttpRequest parse(const std::string& data);
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_REQUEST_PARSER_H
