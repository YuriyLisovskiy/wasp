/**
 * http/mime/multipart/uploaded_file.h
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


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: UploadedFile
// TODO: docs for 'UploadedFile'
class UploadedFile
{
private:
	std::string _read_from_name;
	size_t _size{};
	std::string _data;

	inline explicit UploadedFile(const std::string& name)
	{
		this->_read_from_name = name;
	}

	explicit UploadedFile(size_t size, std::string data);

public:
	inline static UploadedFile from_path(const std::string& name)
	{
		return UploadedFile(name);
	}

	inline static UploadedFile from_content(const std::string& content)
	{
		return UploadedFile(content.size(), content);
	}

	UploadedFile() = default;

	UploadedFile(const UploadedFile& other);

	UploadedFile& operator=(const UploadedFile& other);

	std::string content();

	[[nodiscard]]
	inline std::string read_from_name() const
	{
		return this->_read_from_name;
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

	void save(const std::string& to);
};

__HTTP_MIME_MULTIPART_END__
