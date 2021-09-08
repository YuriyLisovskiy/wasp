/**
 * http/mime/multipart/quote_printable_reader.h
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

// Framework libraries.
// TODO:


__HTTP_MIME_MULTIPART_BEGIN__

// TESTME: QuotePrintableReader
// TODO: docs for 'QuotePrintableReader'
class QuotePrintableReader : public io::IReader
{
public:
	inline explicit QuotePrintableReader(std::shared_ptr<io::IReader> /*reader*/)
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
