/**
 * core/mime_types.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Guess the MIME type of a file.
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
 * std::string ext_from_path(const std::string& path) - retrieves
 * an extension from file path.
 *
 * void guessContentType(const std::string& _path, std::string& type, std::string& encoding) -- guess
 * the MIME type and encoding of a URL.
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/collections/dictionary.h>

// Module definitions.
#include "./_def_.h"


__CORE_MIME_BEGIN__

// TESTME: ext_from_file_name
//
// Returns extension from file name,
//		i.e. 'foo.bar' - 'bar' will be returned.
//
// `file_name`: name to analyze.
extern std::string ext_from_file_name(const std::string& file_name);

// TESTME: ext_from_path
//
// Returns extension from file path,
//		i.e. 'path/to/foo.bar' - 'bar' will be returned.
//
// `path`: path to analyze.
extern std::string ext_from_path(const std::string& path);

// TESTME: guess_content_type
//
// Returns content type from file path.
//
// `_path`: path to analyze.
// `type`: guessed file type.
// `encoding`: guessed file encoding.
extern void guess_content_type(const std::string& _path, std::string& type, std::string& encoding);

// Dictionary which maps reduced archives' extensions to
// their full variants.
extern collections::Dictionary<std::string, std::string> SUFFIX_MAP;

// Dictionary which maps archives' extensions to
//	encoding names.
extern collections::Dictionary<std::string, std::string> ENCODINGS_MAP;

// Before adding new types, make sure they are either registered with IANA,
// at http://www.iana.org/assignments/media-types
// or extensions, i.e. using the x- prefix
extern collections::Dictionary<std::string, std::string> TYPES_MAP;

__CORE_MIME_END__
