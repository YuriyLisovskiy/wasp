/**
 * utils/http.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: provides http utils.
 */

#pragma once

// Core libraries.
#include <xalwart.core/regex.h>

// Module definitions.
#include "./_def_.h"


__UTILS_HTTP_BEGIN__

/// Parse a date format as specified by HTTP RFC7231 section 7.1.1.1.
///
/// The three formats allowed by the RFC are accepted, even if only the first
/// one is still in widespread use.
///
/// Return an integer expressed in seconds since the epoch, in UTC.
extern long parse_http_date(const std::string& date);

/// If the provided string is already a quoted ETag, return it.
/// Otherwise, wrap the string in quotes, making it a strong ETag.
extern std::string quote_etag(const std::string& e_tag);

/// Parse a string of ETags given in an If-None-Match or If-Match header as
/// defined by RFC 7232. Return a std::vector<std::string> of quoted ETags,
/// or {"*"} if all ETags should be matched.
extern std::vector<std::string> parse_etags(const std::string& etag_str);

__UTILS_HTTP_END__


__UTILS_HTTP_INTERNAL_BEGIN__

extern rgx::Regex ETAG_REGEX;

extern const std::string _D;
extern const std::string _D2;
extern const std::string _M;
extern const std::string _Y;
extern const std::string _Y2;
extern const std::string _T;

extern rgx::ArgRegex RFC1123_DATE;
extern rgx::ArgRegex RFC850_DATE;
extern rgx::ArgRegex ASCTIME_DATE;

extern const std::vector<std::string> MONTHS;

__UTILS_HTTP_INTERNAL_END__
