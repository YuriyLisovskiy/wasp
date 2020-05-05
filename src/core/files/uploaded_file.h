/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/files/uploaded_file.h
 *
 * Purpose:
 * 	Represents some file data that the user
 * 	submitted with a form.
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Framework modules.
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

	std::string name() const;
	std::string boundary() const;
	std::string content_disposition() const;
	std::string charset() const;
	std::string content_type() const;
	size_t size() const;
	bool exists() const;
	void save();
};

__CORE_END__
