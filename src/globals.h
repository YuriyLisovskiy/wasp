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

/*
 * This header contains global variables, constants and definitions.
 */

#ifndef WASP_GLOBALS_H
#define WASP_GLOBALS_H


// Main namespace.
//
// Full name: wasp
#define __WASP_BEGIN__ namespace wasp {
#define __WASP_END__ }

// Main internal namespace.
//
// Full name: wasp::internal
#define __INTERNAL_BEGIN__ __WASP_BEGIN__ namespace internal {
#define __INTERNAL_END__ } __WASP_END__

// This namespace is used in datetime module.
//
// Full name: wasp::dt
//#define __DATETIME_BEGIN__ __WASP_BEGIN__ namespace dt {
//#define __DATETIME_END__ } __WASP_END__

// Used for string utility library.
//
// Full name: wasp::str
#define __UTILS_STR_BEGIN__ __WASP_BEGIN__ namespace str {
#define __UTILS_STR_END__ } __WASP_END__

// String utility internals.
//
// Full name: wasp::str::internal
#define __UTILS_STR_INTERNAL_BEGIN__ __UTILS_STR_BEGIN__ namespace internal {
#define __UTILS_STR_INTERNAL_END__ } __UTILS_STR_END__

// Namespace for media types utility functions.
//
// Full name: wasp::mime
#define __MIME_BEGIN__ __WASP_BEGIN__ namespace mime {
#define __MIME_END__ } __WASP_END__

// Media types utility internals.
//
// Full name: wasp::mime::internal
#define __MIME_INTERNAL_BEGIN__ __MIME_BEGIN__ namespace internal {
#define __MIME_INTERNAL_END__ } __MIME_END__

// Path functions namespace.
//
// Full name: wasp::path
#define __PATH_BEGIN__ __WASP_BEGIN__ namespace path {
#define __PATH_END__ } __WASP_END__

// Path library internals.
//
// Full name: wasp::path::internal
#define __PATH_INTERNAL_BEGIN__ __PATH_BEGIN__ namespace internal {
#define __PATH_INTERNAL_END__ } __PATH_END__

// Namespace for encoding lib.
//
// Full name: wasp::encoding
#define __WASP_ENCODING_BEGIN__ __WASP_BEGIN__ namespace encoding {
#define __WASP_ENCODING_END__ } __WASP_END__

// Encoding internals.
//
// Full name: wasp::encoding::internal
#define __WASP_ENCODING_INTERNAL_BEGIN__ __WASP_ENCODING_BEGIN__ namespace internal {
#define __WASP_ENCODING_INTERNAL_END__ } __WASP_ENCODING_END__

// Required parameters for Wasp's built-in logger.
#define _ERROR_DETAILS_ __LINE__, __FUNCTION__, __FILE__

// Pass this when file details is not required.
#define _DETAILS_NONE_ 0, "", ""

__WASP_BEGIN__

typedef unsigned char byte;

__WASP_END__

#endif // WASP_GLOBALS_H
