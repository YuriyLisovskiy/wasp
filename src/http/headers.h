/**
 * http/headers.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Constant names of HTTP headers.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

inline const char* HOST = "Host";

inline const char* USER_AGENT = "User-Agent";

inline const char* CACHE_CONTROL = "Cache-Control";

inline const char* CONTENT_LOCATION = "Content-Location";

inline const char* DATE = "Date";

inline const char* EXPIRES = "Expires";

inline const char* VARY = "Vary";

inline const char* E_TAG = "ETag";

inline const char* IF_MATCH = "If-Match";
inline const char* IF_MODIFIED_SINCE = "If-Modified-Since";
inline const char* IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
inline const char* IF_NONE_MATCH = "If-None-Match";

inline const char* LAST_MODIFIED = "Last-Modified";

inline const char* CONTENT_ENCODING = "Content-Encoding";

inline const char* CONTENT_LENGTH = "Content-Length";

inline const char* REFERRER_POLICY = "Referrer-Policy";

inline const char* STRICT_TRANSPORT_SECURITY = "Strict-Transport-Security";

inline const char* X_FRAME_OPTIONS = "X-Frame-Options";
inline const char* X_FORWARDED_HOST = "X-Forwarded-Host";
inline const char* X_FORWARDED_PORT = "X-Forwarded-Port";
inline const char* X_FORWARDED_PROTO = "X-Forwarded-Proto";
inline const char* X_XSS_PROTECTION = "X-XSS-Protection";
inline const char* X_CONTENT_TYPE_OPTIONS = "X-Content-Type-Options";

__HTTP_END__
