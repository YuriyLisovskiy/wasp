/**
 * http/mime/multipart/quote_printable_reader.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./part_reader.h"

// Base libraries.
#include <xalwart.base/net/_def_.h>

// Framework libraries.
#include "./part.h"
#include "./body_reader.h"


__HTTP_MIME_MULTIPART_BEGIN__

ssize_t PartReader::read(std::string& buffer, size_t max_count)
{
	auto mr = this->part->multipart_reader;

	std::string peek;
	bool ok = true;
	while (this->part->n == 0 && ok)
	{
		auto buffered = mr->buffered_reader->buffered();
		mr->buffered_reader->peek(peek, buffered);
		auto [n, is_ok] = _scan_until_boundary(peek, mr->dash_boundary, mr->nl_dash_boundary, this->part->total);
		this->part->n = n;
		ok = is_ok;
		if (this->part->n == 0 && ok)
		{
			// Force buffered I/O to read more into buffer.
			auto count = std::min<size_t>(buffered + mr->buffered_reader->limit(), max_count);
			count = std::min(count, net::DEFAULT_BUFFER_SIZE);
			mr->buffered_reader->peek(peek, count);
		}
	}

	if (this->part->n == 0)
	{
		return 0;
	}

	auto n = mr->buffered_reader->read(buffer, this->part->n);
	this->part->total += n;
	this->part->n -= n;
	return n;
}

ssize_t _match_after_prefix(const std::string& buf, const std::string& prefix)
{
	if (buf.size() == prefix.size())
	{
		return 0;
	}

	auto c = buf[prefix.size()];
	if (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '-')
	{
		return +1;
	}

	return -1;
}

std::pair<ssize_t, bool> _scan_until_boundary(
	const std::string& buf, const std::string& dash_boundary, const std::string& nl_dash_boundary, ssize_t total
)
{
	if (total == 0)
	{
		// At beginning of body, allow dash_boundary.
		if (buf.starts_with(dash_boundary))
		{
			switch (_match_after_prefix(buf, dash_boundary))
			{
				case -1:
					return {dash_boundary.size(), true};
				case 0:
					return {0, true};
				case +1:
					return {0, false};
			}
		}

		if (dash_boundary.starts_with(buf))
		{
			return {0, true};
		}
	}

	// Search for "\n--boundary".
	auto i = buf.find(nl_dash_boundary);
	if (i != std::string::npos)
	{
		switch (_match_after_prefix(buf.substr(i), nl_dash_boundary))
		{
			case -1:
				return {i + nl_dash_boundary.size(), true};
			case 0:
				return {i, true};
			case +1:
				return {i, false};
		}
	}

	if (nl_dash_boundary.starts_with(buf))
	{
		return {0, true};
	}

	// Otherwise, anything up to the final \n is not part of the boundary
	// and so must be part of the body.
	// Also if the section from the final \n onward is not a prefix of the boundary,
	// it too must be part of the body.
	i = buf.find_last_of(nl_dash_boundary[0]);
	if (i != std::string::npos && nl_dash_boundary.starts_with(buf.substr(i)))
	{
		return {i, true};
	}

	return {buf.size(), true};
}

__HTTP_MIME_MULTIPART_END__
