/**
 * core/uploaded_file.cpp
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 */

#include "./uploaded_file.h"


__CORE_BEGIN__

UploadedFile::UploadedFile(
	const std::string& name,
	size_t size,
	File& file,
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
	this->_file = File(file);
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
		this->_file = copy._file;
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
		this->_file = copy._file;
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
	if (!this->_file.path().empty())
	{
		this->_file.save_file();
	}
}

__CORE_END__
