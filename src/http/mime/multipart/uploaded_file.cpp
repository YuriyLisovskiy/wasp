/**
 * http/mime/multipart/uploaded_file.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./uploaded_file.h"


__HTTP_MIME_MULTIPART_BEGIN__

UploadedFile::UploadedFile(size_t size, std::string data)
{
	this->_size = size;
	if (this->_size > 0)
	{
		this->_data = std::move(data);
	}
}

UploadedFile::UploadedFile(const UploadedFile& other)
{
	if (this != &other)
	{
		this->_read_from_name = other._read_from_name;
		this->_size = other._size;
		if (this->_size > 0)
		{
			this->_data = other._data;
		}
	}
}

UploadedFile& UploadedFile::operator= (const UploadedFile& other)
{
	if (this != &other)
	{
		this->_read_from_name = other._read_from_name;
		this->_size = other._size;
		if (this->_size > 0)
		{
			this->_data = other._data;
		}
	}

	return *this;
}

std::string UploadedFile::content()
{
	if (!this->_read_from_name.empty())
	{
		File file(this->_read_from_name, File::OpenMode::ReadBinary);
		file.open();
		if (file.is_open())
		{
			this->_data = file.read_str();
			file.close();
			this->_size = this->_data.size();
		}
	}

	return this->_data;
}

void UploadedFile::save(const std::string& to)
{
	File file(to, File::OpenMode::WriteBinary);
	file.open();
	if (file.is_open())
	{
		file.write(this->content());
		file.save();
	}
}

__HTTP_MIME_MULTIPART_END__
