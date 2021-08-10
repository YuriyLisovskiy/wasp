/**
 * http/headers.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Http headers constants.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

inline const std::string HOST = "Host";

inline const std::string USER_AGENT = "User-Agent";

inline const std::string CACHE_CONTROL = "Cache-Control";

inline const std::string CONTENT_LOCATION = "Content-Location";

inline const std::string DATE = "Date";

inline const std::string EXPIRES = "Expires";

inline const std::string VARY = "Vary";

inline const std::string E_TAG = "ETag";

inline const std::string IF_MATCH = "If-Match";
inline const std::string IF_MODIFIED_SINCE = "If-Modified-Since";
inline const std::string IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
inline const std::string IF_NONE_MATCH = "If-None-Match";

inline const std::string LAST_MODIFIED = "Last-Modified";

inline const std::string CONTENT_ENCODING = "Content-Encoding";

inline const std::string CONTENT_LENGTH = "Content-Length";

inline const std::string REFERRER_POLICY = "Referrer-Policy";

inline const std::string STRICT_TRANSPORT_SECURITY = "Strict-Transport-Security";

inline const std::string X_FRAME_OPTIONS = "X-Frame-Options";
inline const std::string X_FORWARDED_HOST = "X-Forwarded-Host";
inline const std::string X_FORWARDED_PORT = "X-Forwarded-Port";
inline const std::string X_FORWARDED_PROTO = "X-Forwarded-Proto";
inline const std::string X_XSS_PROTECTION = "X-XSS-Protection";
inline const std::string X_CONTENT_TYPE_OPTIONS = "X-Content-Type-Options";

__HTTP_END__
