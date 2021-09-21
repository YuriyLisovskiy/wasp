/**
 * http/mime/multipart/form.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Multipart request parser utilities.
 *
 * Implementation is based on Golang 1.15.8: mime/multipart/formdata.go
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>
#include <filesystem>

// Base libraries.
#include <xalwart.base/collections/multimap.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./file_header.h"


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: Form
//
// It is a parsed multipart form.
// Its `files` parts are stored either in memory or on disk,
// and are accessible via the `FileHeader`'s `open` method.
// Its `values` parts are stored as strings.
// Both are keyed by field name.
struct Form final
{
	collections::multimap<std::string, std::string> values;
	collections::multimap<std::string, std::shared_ptr<FileHeader>> files;

	// Removes any temporary files associated with a `Form`.
	inline void remove_all() const
	{
		for (const auto& fhs : this->files)
		{
			for (const auto& fh : fhs.second)
			{
				if (fh && !fh->tmp_file.empty())
				{
					std::filesystem::remove(fh->tmp_file);
				}
			}
		}
	}
};

__HTTP_MIME_MULTIPART_END__
