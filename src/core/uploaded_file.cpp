/**
 * core/uploaded_file.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./uploaded_file.h"


__CORE_FILES_BEGIN__

UploadedFile::UploadedFile(
	const std::string& name,
	size_t size,
	std::vector<unsigned char>& data,
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
	if (this->_size > 0)
	{
		this->_data = std::move(data);
	}
}

UploadedFile::UploadedFile(const UploadedFile& other)
{
	if (this != &other)
	{
		this->_name = other._name;
		this->_boundary = other._boundary;
		this->_content_disposition = other._content_disposition;
		this->_charset = other._charset;
		this->_content_type = other._content_type;
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
		this->_name = other._name;
		this->_boundary = other._boundary;
		this->_content_disposition = other._content_disposition;
		this->_charset = other._charset;
		this->_content_type = other._content_type;
		this->_size = other._size;
		if (this->_size > 0)
		{
			this->_data = other._data;
		}
	}

	return *this;
}

std::string UploadedFile::name() const
{
	return this->_name;
}

std::string UploadedFile::boundary() const
{
	return this->_boundary;
}

std::string UploadedFile::content_disposition() const
{
	return this->_content_disposition;
}

std::string UploadedFile::charset() const
{
	return this->_charset;
}

std::string UploadedFile::content_type() const
{
	return this->_content_type;
}

size_t UploadedFile::size() const
{
	return this->_size;
}

bool UploadedFile::exists() const
{
	return this->_size != 0;
}

void UploadedFile::save()
{
	if (this->_name != "/")
	{
		File file(this->_name, File::wb);
		file.open();
		if (file.is_open())
		{
			file.write(this->_data);
			file.save();
		}
	}
}

__CORE_FILES_END__
