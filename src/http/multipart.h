/**
 * http/multipart.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <memory>

// Base libraries.
#include <xalwart.base/io.h>
#include <xalwart.base/collections/multi_dictionary.h>
#include <xalwart.base/path.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./errors.h"
#include "../core/uploaded_file.h"


__HTTP_MULTIPART_BEGIN__

const long long int DEFAULT_MAX_MEMORY = 32 << 20; // 32 MB

// TESTME: FileHeader
// TODO: docs for 'FileHeader'
class FileHeader final
{
public:
	std::string filename;
	collections::MultiDictionary<std::string, std::string> header;
	unsigned long long int size;

	std::string content;
	std::string tmpfile;

	[[nodiscard]]
	files::UploadedFile open() const;
};

// TESTME: Form
// TODO: docs for 'Form'
class Form
{
public:
	collections::MultiDictionary<std::string, std::string> values;
	collections::MultiDictionary<std::string, std::shared_ptr<FileHeader>> files;

	inline void remove_all() const;
};

// TESTME: Part
// TODO: docs for 'Part'
class Part
{
public:
	collections::MultiDictionary<std::string, std::string> header;

	inline std::string form_name()
	{
		// TODO:
		return "";
	}

	inline std::string file_name()
	{
		// TODO:
		return "";
	}

	inline std::pair<long long int, ParseFormError> read(std::string& buffer, long long int max_n)
	{
		// TODO:
		return {0, ParseFormError::Nil};
	}
};

// TESTME: Reader
// TODO: docs for 'Reader'
class Reader
{
public:
	inline Reader(io::IReader* body_reader, const std::string& boundary)
	{
		// TODO:
	}

	std::tuple<std::unique_ptr<Form>, ParseFormError> read_form(long long int max_memory);

	inline std::tuple<std::unique_ptr<Part>, ParseFormError> next_part()
	{
		// TODO:
	}
};

__HTTP_MULTIPART_END__
