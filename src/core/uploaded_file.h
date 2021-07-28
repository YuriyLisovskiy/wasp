/**
 * core/uploaded_file.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Represents some file data that the user submitted with a form.
 */

#pragma once

// Base modules.
#include <xalwart.base/file.h>

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

	[[nodiscard]]
	inline std::string name() const
	{
		return this->_name;
	}

	[[nodiscard]]
	inline std::string boundary() const
	{
		return this->_boundary;
	}

	[[nodiscard]]
	inline std::string content_disposition() const
	{
		return this->_content_disposition;
	}

	[[nodiscard]]
	inline std::string charset() const
	{
		return this->_charset;
	}

	[[nodiscard]]
	inline std::string content_type() const
	{
		return this->_content_type;
	}

	[[nodiscard]]
	inline size_t size() const
	{
		return this->_size;
	}

	[[nodiscard]]
	inline bool exists() const
	{
		return this->_size != 0;
	}

	void save();
};

__CORE_FILES_END__
