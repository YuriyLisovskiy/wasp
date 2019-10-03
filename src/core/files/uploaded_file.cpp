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

#include "uploaded_file.h"


__WASP_BEGIN__

UploadedFile::UploadedFile(
	const std::string& name,
	size_t size,
	const std::string& contentType,
	const std::string& charset
)
{
	this->_name = name;
	this->_contentType = contentType;
	this->_charset = charset;
	this->_size = size;
}

std::string UploadedFile::getName()
{
	return this->_name;
}

std::string UploadedFile::getCharset()
{
	return this->_charset;
}

std::string UploadedFile::getContentType()
{
	return this->_contentType;
}

size_t UploadedFile::getSize()
{
	return this->_size;
}

bool UploadedFile::exists()
{
	struct stat buffer{};
	return stat(this->_name.c_str(), &buffer) == 0;
}

__WASP_END__
