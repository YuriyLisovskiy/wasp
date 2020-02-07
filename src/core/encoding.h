/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * core/encoding.h
 *
 * Purpose: encode/decode strings.
 */

#pragma once

// C++ libraries.
#include <string>
#include <sstream>
#include <iomanip>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../core/exceptions.h"


__ENCODING_BEGIN__

/// Encodes url using percent-encoding.
extern std::string encode_url(const std::string& url);

extern std::string quote(const std::string& _str, const std::string& safe = "");

extern const uint ASCII;

enum Mode
{
	STRICT, IGNORE, REPLACE
};

/// Encode string to given encoding.
///
/// Available encodings:
///  - ascii
///
/// Modes:
///  - strict: throws EncodingError if string violates encoding rules;
///  - ignore: removes offending symbols from string;
///  - replace: replaces offending symbols by question mark ('?').
extern std::string encode(const std::string& _str, uint encoding, Mode mode = Mode::STRICT);

extern std::string encode_ascii(const std::string& _str, Mode mode);

__ENCODING_END__


__ENCODING_INTERNAL_BEGIN__

/// Converts character to percent-encoded character and writes it to stream.
///
/// @safe - string which contains chars that must not be converted. If char 'c'
/// is in this sequence, it will be written to stream without converting.
///
/// RFC 3986 section 2.2 Reserved Characters: ! * ' ( ) ; : @ & = + $ , / ? # [ ]
/// RFC 3986 section 2.3 Unreserved Characters: ALPHA NUMERIC - _ . ~
extern void escape(std::ostringstream& stream, char c, const std::string& safe = "");

__ENCODING_INTERNAL_END__
