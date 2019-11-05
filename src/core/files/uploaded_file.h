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
 * An 'UploadedFile' object represents some file data that the user
 *  submitted with a form.
 */

#ifndef WASP_CORE_FILES_UPLOADED_FILE_H
#define WASP_CORE_FILES_UPLOADED_FILE_H

#include "../../globals.h"


__WASP_BEGIN__

class UploadedFile
{
private:
	std::string _name;
	std::string _boundary;
	std::string _contentDisposition;
	std::string _charset;
	std::string _contentType;
	size_t _size{};

public:
	UploadedFile(
		const std::string& name,
		size_t size,
		const std::string& contentType = "",
		const std::string& charset = "",
		const std::string& boundary = "",
		const std::string& contentDisposition = ""
	);
	UploadedFile() = default;
	UploadedFile(const UploadedFile& copy)
	{
		if (this != &copy)
		{
			this->_name = copy._name;
			this->_boundary = copy._boundary;
			this->_contentDisposition = copy._contentDisposition;
			this->_charset = copy._charset;
			this->_contentType = copy._contentType;
			this->_size = copy._size;
		}
	}

	UploadedFile& operator=(const UploadedFile& copy)
	{
		if (this != &copy)
		{
			this->_name = copy._name;
			this->_boundary = copy._boundary;
			this->_contentDisposition = copy._contentDisposition;
			this->_charset = copy._charset;
			this->_contentType = copy._contentType;
			this->_size = copy._size;
		}

		return *this;
	}

	std::string name();
	std::string boundary();
	std::string contentDisposition();
	std::string charset();
	std::string contentType();
	size_t size();
	bool exists();
};

__WASP_END__


#endif // WASP_CORE_FILES_UPLOADED_FILE_H
