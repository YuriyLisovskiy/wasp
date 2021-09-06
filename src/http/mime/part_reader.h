/**
 * http/mime/quote_printable_reader.h
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

// Base libraries.
#include <xalwart.base/io.h>
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"


__HTTP_MULTIPART_BEGIN__

class Part;

// TESTME: PartReader
//
// Implements `io::IReader` by reading raw bytes directly from the
// wrapped `Part*`, without doing any Transfer-Encoding decoding.
class PartReader : public io::IReader
{
public:
	inline explicit PartReader(Part* part) : part(part)
	{
		if (!this->part)
		{
			throw NullPointerException("'part' is nullptr", _ERROR_DETAILS_);
		}
	}

	inline ssize_t read_line(std::string& buffer) override
	{
		throw NotImplementedException("PartReader::read_line(std::string& buffer)", _ERROR_DETAILS_);
	}

	ssize_t read(std::string& buffer, size_t max_count) override;

	inline bool close_reader() override
	{
		throw NotImplementedException("PartReader::close_reader()", _ERROR_DETAILS_);
	}

protected:
	Part* part;
};

// TESTME: _match_after_prefix
//
// Checks whether `buf` should be considered to match the boundary.
// The `prefix` is "--boundary" or "\r\n--boundary" or "\n--boundary",
// and the caller has verified already that `buf.starts_with(prefix)` is true.
//
// Returns +1 if the buffer does match the boundary,
// meaning the prefix is followed by a dash, space, tab, cr, nl, or end of input.
// It returns -1 if the buffer definitely does NOT match the boundary,
// meaning the `prefix` is followed by some other character.
// For example, "--foobar" does not match "--foo".
// It returns 0 more input needs to be read to make the decision,
// meaning that `buf.size() == prefix.size()`.
extern ssize_t _match_after_prefix(const std::string& buf, const std::string& prefix);

// TESTME: _scan_until_boundary
//
// Scans `buf` to identify how much of it can be safely
// returned as part of the `Part` body.
// `dash_boundary` is "--boundary".
// `nl_dash_boundary` is "\r\n--boundary" or "\n--boundary", depending on what mode we are in.
// The comments below (and the name) assume "\n--boundary", but either is accepted.
// total is the number of bytes read out so far. If total == 0, then a leading "--boundary" is recognized.
extern std::pair<ssize_t, bool> _scan_until_boundary(
	const std::string& buf, const std::string& dash_boundary, const std::string& nl_dash_boundary, ssize_t total
);

__HTTP_MULTIPART_END__
