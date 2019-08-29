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
 * MultipartParser definition.
 * TODO: write docs.
 */

#ifndef WASP_HTTP_PARSERS_MULTIPART_PARSER_H
#define WASP_HTTP_PARSERS_MULTIPART_PARSER_H

#include <string>
#include <functional>

#include "../../globals.h"
#include "../../utils/str.h"
#include "../../core/exceptions.h"
#include "../../collections/dict.h"
#include "../../collections/multi_dict.h"


__INTERNAL_BEGIN__

typedef std::function<void(const std::string&, const std::string&)> UploadHandler;

class MultipartParser
{
private:
	UploadHandler _uploadHandler;

	Dict<std::string, std::string> _dict;
	MultiValueDict<std::string, std::string> _multiDict;

protected:
	enum ParserState
	{
		BoundaryBegin,
		Boundary,
		BoundaryEnd,
		BodyEnd,
		ContentDispositionBegin,
		ContentDisposition,
		NameBegin,
		Name,
		NameEnd,
		FileNameBegin,
		FileName,
		ContentTypeBegin,
		ContentType,
		ContentBegin,
		Content

	} _state;

	void _appendParameter(const std::string& key, const std::string& value);
//	void _appendFile(const std::string& key, const std::string& value);

	static std::string _getBoundary(const std::string& contentType);
	static void _assertBoundary(const std::string& actual, const std::string& expected);

public:
	explicit MultipartParser(const UploadHandler& uploadHandler);
	void parse(const std::string& contentType, const std::string& body);
	MultiValueDict<std::string, std::string> getFilesParameters();
	Dict<std::string, std::string> getPostParameters();
};

__INTERNAL_END__


#endif // WASP_HTTP_PARSERS_MULTIPART_PARSER_H
