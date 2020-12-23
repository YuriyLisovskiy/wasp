/**
 * http/headers.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: http headers constants.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"


__HTTP_BEGIN__

const std::string HOST = "Host";

const std::string USER_AGENT = "User-Agent";

const std::string CACHE_CONTROL = "Cache-Control";

const std::string CONTENT_LOCATION = "Content-Location";

const std::string DATE = "Date";

const std::string EXPIRES = "Expires";

const std::string VARY = "Vary";

const std::string E_TAG = "ETag";

const std::string IF_MATCH = "If-Match";
const std::string IF_MODIFIED_SINCE = "If-Modified-Since";
const std::string IF_UNMODIFIED_SINCE = "If-Unmodified-Since";
const std::string IF_NONE_MATCH = "If-None-Match";

const std::string LAST_MODIFIED = "Last-Modified";

const std::string CONTENT_ENCODING = "Content-Encoding";

const std::string CONTENT_LENGTH = "Content-Length";

const std::string REFERRER_POLICY = "Referrer-Policy";

const std::string STRICT_TRANSPORT_SECURITY = "Strict-Transport-Security";

const std::string X_FRAME_OPTIONS = "X-Frame-Options";
const std::string X_FORWARDED_HOST = "X-Forwarded-Host";
const std::string X_XSS_PROTECTION = "X-XSS-Protection";
const std::string X_CONTENT_TYPE_OPTIONS = "X-Content-Type-Options";

__HTTP_END__
