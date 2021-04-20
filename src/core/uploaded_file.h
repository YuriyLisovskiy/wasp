/**
 * core/uploaded_file.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Represents some file data that the user submitted with a form.
 */

#pragma once

// Core modules.
#include <xalwart.core/file.h>

// Module definitions.
#include "./_def_.h"


__CORE_FILES_BEGIN__

// TESTME: UploadedFile
class UploadedFile
{
private:
	std::string _name;
	std::string _boundary;
	std::string _content_disposition;
	std::string _charset;
	std::string _content_type;
	size_t _size{};
	std::vector<unsigned char> _data;

public:
	UploadedFile() = default;
	UploadedFile(
		const std::string& name,
		size_t size,
		std::vector<unsigned char>& data,
		const std::string& content_type = "",
		const std::string& charset = "",
		const std::string& boundary = "",
		const std::string& content_disposition = ""
	);
	UploadedFile(const UploadedFile& other);
	UploadedFile& operator=(const UploadedFile& other);

	[[nodiscard]] std::string name() const;
	[[nodiscard]] std::string boundary() const;
	[[nodiscard]] std::string content_disposition() const;
	[[nodiscard]] std::string charset() const;
	[[nodiscard]] std::string content_type() const;
	[[nodiscard]] size_t size() const;
	[[nodiscard]] bool exists() const;
	void save();
};

__CORE_FILES_END__
