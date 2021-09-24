/**
 * http/mime/multipart/base64_reader.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * TODO: purpose
 *
 * Implementation is based on Golang 1.15.8: mime/quoteprintable/reader.go
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>

// Base libraries.
#include <xalwart.base/io.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: Base64Reader
// TODO: docs for 'Base64Reader'
class Base64Reader : public io::IReader
{
public:
	inline explicit Base64Reader(std::shared_ptr<io::IReader> /*reader*/)
	{
		// TODO:
		throw NotImplementedException("QuotePrintableReader()", _ERROR_DETAILS_);
	}

	inline ssize_t read_line(std::string& buffer) override
	{
		// TODO:
		throw NotImplementedException("read_line()", _ERROR_DETAILS_);
	}

	inline ssize_t read(std::string& buffer, size_t max_count) override
	{
		// TODO:
		throw NotImplementedException("read()", _ERROR_DETAILS_);
	}

	inline bool close_reader() override
	{
		// TODO:
		throw NotImplementedException("close_reader()", _ERROR_DETAILS_);
	}
};

__HTTP_MIME_MULTIPART_END__
