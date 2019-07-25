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

/* Guess the MIME type of a file.
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
 * std::string extFromPath(const std::string& path) - retrieves an extension from file path.
 *
 * void guessContentType(const std::string& _path, std::string& type, std::string& encoding) -- guess
 *  the MIME type and encoding of a URL.
 */

#ifndef WASP_UTILS_MIME_TYPES_H
#define WASP_UTILS_MIME_TYPES_H

#include <string>

#include "../globals.h"
#include "str.h"
#include "../collections/dict.h"
#include "path.h"


__MIME_BEGIN__

// Returns extension from file name,
// i.e. 'foo.bar' - 'bar' will be returned.
extern std::string extFromFileName(const std::string& fileName);

// Returns extension from file path,
// i.e. 'path/to/foo.bar' - 'bar' will be returned.
extern std::string extFromPath(const std::string& path);

// Returns content type from file path.
extern void guessContentType(const std::string& _path, std::string& type, std::string& encoding);

extern Dict<std::string, std::string> SUFFIX_MAP;

extern Dict<std::string, std::string> ENCODINGS_MAP;

// Before adding new types, make sure they are either registered with IANA,
// at http://www.iana.org/assignments/media-types
// or extensions, i.e. using the x- prefix
extern Dict<std::string, std::string> TYPES_MAP;

__MIME_END__


#endif // WASP_UTILS_MIME_TYPES_H
