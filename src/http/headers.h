/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * headers.h
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

const std::string E_TAG = "ETag";

const std::string IF_MODIFIED_SINCE = "If-Modified-Since";

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
