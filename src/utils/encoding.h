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
 * encoding definition.
 * TODO: write docs.
 */

#ifndef WASP_ENCODING_H
#define WASP_ENCODING_H

#import <string>

#include "../globals.h"


__INTERNAL_BEGIN__

// Convert an Internationalized Resource Identifier (IRI) portion to a URI
// portion that is suitable for inclusion in a URL.
// This is the algorithm from section 3.1 of RFC 3987, slightly simplified
// since the input is assumed to be a string rather than an arbitrary byte
// stream.
//
// Take an IRI string, e.g. '/I ♥ Wasp/' and return a string containing the
// encoded result with ASCII chars only (e.g. '/I%20%E2%99%A5%20Wasp/').
//
// The list of safe characters here is constructed from the "reserved" and
// "unreserved" characters specified in sections 2.2 and 2.3 of RFC 3986:
//     reserved    = gen-delims / sub-delims
//     gen-delims  = ":" / "/" / "?" / "#" / "[" / "]" / "@"
//     sub-delims  = "!" / "$" / "&" / "'" / "(" / ")"
//                   / "*" / "+" / "," / ";" / "="
//     unreserved  = ALPHA / DIGIT / "-" / "." / "_" / "~"
//
// The % character is also added to the list of safe characters here, as the
// end of section 3.1 of RFC 3987 specifically mentions that % must not be
// converted.
extern std::string iriToUri(const std::string& iri);

__INTERNAL_END__


#endif // WASP_ENCODING_H
