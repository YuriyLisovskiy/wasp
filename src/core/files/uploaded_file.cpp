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
	const std::string& content_type,
	const std::string& charset,
	const std::string& boundary,
	const std::string& content_disposition
)
{
	this->_name = name;
	this->_boundary = boundary;
	this->_content_disposition = content_disposition;
	this->_content_type = content_type;
	this->_charset = charset;
	this->_size = size;
}

UploadedFile::UploadedFile(const UploadedFile& copy)
{
	if (this != &copy)
	{
		this->_name = copy._name;
		this->_boundary = copy._boundary;
		this->_content_disposition = copy._content_disposition;
		this->_charset = copy._charset;
		this->_content_type = copy._content_type;
		this->_size = copy._size;
	}
}

UploadedFile& UploadedFile::operator=(const UploadedFile& copy)
{
	if (this != &copy)
	{
		this->_name = copy._name;
		this->_boundary = copy._boundary;
		this->_content_disposition = copy._content_disposition;
		this->_charset = copy._charset;
		this->_content_type = copy._content_type;
		this->_size = copy._size;
	}

	return *this;
}

std::string UploadedFile::name()
{
	return this->_name;
}

std::string UploadedFile::boundary()
{
	return this->_boundary;
}

std::string UploadedFile::content_disposition()
{
	return this->_content_disposition;
}

std::string UploadedFile::charset()
{
	return this->_charset;
}

std::string UploadedFile::content_type()
{
	return this->_content_type;
}

size_t UploadedFile::size()
{
	return this->_size;
}

bool UploadedFile::exists()
{
	return this->_size != 0;
}

__WASP_END__
