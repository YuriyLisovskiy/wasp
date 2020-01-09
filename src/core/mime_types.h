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
 * mime_types.h
 *
 * Purpose: guess the MIME type of a file.
 *
 * Variables:
 *
 * SUFFIX_MAP -- dictionary mapping suffixes to suffixes
 * ENCODINGS_MAP -- dictionary mapping suffixes to encodings
 * TYPES_MAP -- dictionary mapping suffixes to types
 *
 *
 * Functions:
 *
 * std::string ext_from_path(const std::string& path) - retrieves an extension from file path.
 *
 * void guessContentType(const std::string& _path, std::string& type, std::string& encoding) -- guess
 *  the MIME type and encoding of a URL.
 */

#pragma once

// C++ libraries.
#include <string>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "./path.h"
#include "./string/str.h"
#include "../collections/dict.h"


__MIME_BEGIN__

/// Returns extension from file name,
///		i.e. 'foo.bar' - 'bar' will be returned.
///
/// @param file_name: name to analyze.
/// @return extension of given file's name.
extern std::string ext_from_file_name(const std::string& file_name);

/// Returns extension from file path,
///		i.e. 'path/to/foo.bar' - 'bar' will be returned.
///
/// @param path: path to analyze.
/// @return extension of given file's path.
extern std::string ext_from_path(const std::string& path);

/// Returns content type from file path.
///
/// @param _path: path to analyze.
/// @param type: guessed file type.
/// @param encoding: guessed file encoding.
extern void guess_content_type(const std::string& _path, std::string& type, std::string& encoding);

/// Dictionary which maps reduced archives' extensions to
/// 	their full variants.
extern collections::Dict<std::string, std::string> SUFFIX_MAP;

/// Dictionary which maps archives' extensions to
///		encoding names.
extern collections::Dict<std::string, std::string> ENCODINGS_MAP;

/// Before adding new types, make sure they are either registered with IANA,
/// at http://www.iana.org/assignments/media-types
/// or extensions, i.e. using the x- prefix
extern collections::Dict<std::string, std::string> TYPES_MAP;

__MIME_END__
