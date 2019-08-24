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

MultipartParser::MultipartParser(const UploadHandler& uploadHandler)
{
	this->_uploadHandler = uploadHandler;
}

void MultipartParser::parse(const std::string& contentType, const std::string& body)
{
	auto pos = contentType.find("boundary=");
	if (pos == std::string::npos)
	{
		return;
	}
	auto boundary = contentType.substr(pos + 9);

	// TODO: parse multipart data
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
