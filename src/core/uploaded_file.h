/**
 * core/uploaded_file.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Represents some file data that the user submitted with a form.
 */

#pragma once

// Core modules.
#include <xalwart.core/file.h>

// Module definitions.
#include "./_def_.h"


__FILES_BEGIN__

class UploadedFile
{
private:
	std::string _name;
	std::string _boundary;
	std::string _content_disposition;
	std::string _charset;
	std::string _content_type;
	size_t _size{};
	core::File _file;

public:
	UploadedFile() = default;
	UploadedFile(
		const std::string& name,
		size_t size,
		core::File& file,
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

__FILES_END__
