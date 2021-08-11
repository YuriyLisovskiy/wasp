/**
 * http/status.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * All available http statuses, which contains status code,
 * reason phrase and brief explanation.
 */

#pragma once

// C++ libraries.
#include <string>

// Base modules.
#include <xalwart.base/collections/dictionary.h>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

struct Status
{
	unsigned short int code;
	const char* phrase;
	const char* description;
};

inline constexpr size_t STATUSES_LENGTH = 57;

inline static constexpr Status STATUSES[STATUSES_LENGTH] =
{
	// Informational
	{100, "Continue", "Request received, please continue"},
	{101, "Switching Protocols", "Switching to new protocol; obey Upgrade header"},
	{102, "Processing", ""},

	// Success
	{200, "OK", "Request fulfilled, document follows"},
	{201, "Created", "Document created, URL follows"},
	{202, "Accepted", "Request accepted, processing continues off-line"},
	{203, "Non-Authoritative Information", "Request fulfilled from cache"},
	{204, "No Content", "Request fulfilled, nothing follows"},
	{205, "Reset Content", "Clear input form for further input"},
	{206, "Partial Content", "Partial content follows"},
	{207, "Multi-Status", ""},
	{208, "Already Reported", ""},
	{226, "IM Used", ""},

	// Redirection
	{300, "Multiple Choices", "Object has several resources -- see URI list"},
	{301, "Moved Permanently", "Object moved permanently -- see URI list"},
	{302, "Found", "Object moved temporarily -- see URI list"},
	{303, "See Other", "Object moved -- see Method and URL list"},
	{304, "Not Modified", "Document has not changed since given time"},
	{305, "Use Proxy", "You must use proxy specified in Location to access this resource"},
	{307, "Temporary Redirect", "Object moved temporarily -- see URI list"},
	{308, "Permanent Redirect", "Object moved temporarily -- see URI list"},

	// Client error
	{400, "Bad Request", "Bad request syntax or unsupported method"},
	{401, "Unauthorized", "No permission -- see authorization schemes"},
	{402, "Payment Required", "No payment -- see charging schemes"},
	{403, "Forbidden", "Request forbidden -- authorization will not help"},
	{404, "Not Found", "Nothing matches the given URI"},
	{405, "Method Not Allowed", "Specified method is invalid for this resource"},
	{406, "Not Acceptable", "URI not available in preferred format"},
	{407, "Proxy Authentication Required", "You must authenticate with this proxy before proceeding"},
	{408, "Request Timeout", "Request timed out; try again later"},
	{409, "Conflict", "Request conflict"},
	{410, "Gone", "URI no longer exists and has been permanently removed"},
	{411, "Length Required", "Client must specify Content-Length"},
	{412, "Precondition Failed", "Precondition in headers is false"},
	{413, "Request Entity Too Large", "Entity is too large"},
	{414, "Request-URI Too Long", "URI is too long"},
	{415, "Unsupported Media Type", "Entity body in unsupported format"},
	{416, "Requested Range Not Satisfiable", "Cannot satisfy request range"},
	{417, "Expectation Failed", "Expect condition could not be satisfied"},
	{422, "Unprocessable Entity", ""},
	{423, "Locked", ""},
	{424, "Failed Dependency", ""},
	{426, "Upgrade Required", ""},
	{428, "Precondition Required", "The origin server requires the request to be conditional"},
	{429, "Too Many Requests", "The user has sent too many requests in a given amount of time (\"rate limiting\")"},
	{431, "Request Header Fields Too Large", "The server is unwilling to process the request because its header fields are too large"},

	// Server error
	{500, "Internal Server Error", "Server got itself in trouble"},
	{501, "Not Implemented", "Server does not support this operation"},
	{502, "Bad Gateway", "Invalid responses from another server/proxy"},
	{503, "Service Unavailable", "The server cannot process the request due to a high load"},
	{504, "Gateway Timeout", "The gateway server did not receive a timely response"},
	{505, "HTTP Version Not Supported", "Cannot fulfill request"},
	{506, "Variant Also Negotiates", ""},
	{507, "Insufficient Storage", ""},
	{508, "Loop Detected", ""},
	{510, "Not Extended", ""},
	{511, "Network Authentication Required", "The client needs to authenticate to gain network access"},
};

// TESTME: get_status_by_code
// TODO: docs for 'get_status_by_code'
inline constexpr Status get_status_by_code(unsigned short int code)
{
	auto comparator = [](const auto& l, const auto& r) -> auto {
		return l.code < r.code;
	};
	auto value = Status{code, "Unknown Status", ""};
	auto end = STATUSES + STATUSES_LENGTH;
	auto status = std::lower_bound(STATUSES, end, value, comparator);
	if (status != end && !comparator(value, *status))
	{
		return *status;
	}

	return value;
}

__HTTP_END__
