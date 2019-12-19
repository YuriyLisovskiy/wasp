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

/**
 * uploaded_file.h
 * Purpose: represents some file data that the user
 * 			submitted with a form.
 */

#ifndef WASP_CORE_FILES_UPLOADED_FILE_H
#define WASP_CORE_FILES_UPLOADED_FILE_H

// C++ libraries.
#include <string>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "./file.h"


__CORE_BEGIN__

class UploadedFile
{
private:
	std::string _name;
	std::string _boundary;
	std::string _content_disposition;
	std::string _charset;
	std::string _content_type;
	size_t _size{};
	File _file;

public:
	UploadedFile() = default;
	UploadedFile(
		const std::string& name,
		size_t size,
		File& file,
		const std::string& content_type = "",
		const std::string& charset = "",
		const std::string& boundary = "",
		const std::string& content_disposition = ""
	);
	UploadedFile(const UploadedFile& other);
	UploadedFile& operator=(const UploadedFile& other);

	std::string name();
	std::string boundary();
	std::string content_disposition();
	std::string charset();
	std::string content_type();
	size_t size();
	bool exists();
	void save();
};

__CORE_END__


#endif // WASP_CORE_FILES_UPLOADED_FILE_H
