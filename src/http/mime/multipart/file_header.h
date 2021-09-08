/**
 * http/mime/multipart/file_header.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Meta object for uploaded file.
 *
 * Implementation is based on Golang 1.15.8: mime/multipart/formdata.go
 */

#pragma once

// C++ libraries.
#include <string>
#include <map>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./uploaded_file.h"


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: FileHeader
// Describes a file part of a multipart request.
class FileHeader final
{
public:
	std::string filename;
	std::map<std::string, std::string> header;
	unsigned long long int size;

	std::string content;
	std::string tmp_file;

	// Opens and returns the `FileHeader`'s associated `UploadedFile`.
	[[nodiscard]]
	inline UploadedFile open() const
	{
		if (this->content.empty())
		{
			return UploadedFile::from_path(this->tmp_file);
		}

		return UploadedFile::from_content(this->content);
	}
};

__HTTP_MIME_MULTIPART_END__
