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

#include <sys/stat.h>

#include "../../globals.h"


__WASP_BEGIN__

class UploadedFile
{
private:
	std::string _name;
	std::string _charset;
	std::string _contentType;
	size_t _size{};

public:
	UploadedFile(
		const std::string& name,
		size_t size,
		const std::string& contentType = "",
		const std::string& charset = ""
	);
	UploadedFile() = default;
	UploadedFile(const UploadedFile& copy)
	{
		if (this != &copy)
		{
			this->_name = copy._name;
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
			this->_charset = copy._charset;
			this->_contentType = copy._contentType;
			this->_size = copy._size;
		}

		return *this;
	}

	std::string getName();
	std::string getCharset();
	std::string getContentType();
	size_t getSize();
	bool exists();
};

__WASP_END__


#endif // WASP_CORE_FILES_UPLOADED_FILE_H
