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
 * path definition.
 * TODO: write docs.
 */

#ifndef WASP_UTILS_PATH_H
#define WASP_UTILS_PATH_H

#include <string>

#include "../globals.h"


__PATH_INTERNAL_BEGIN__

// Split a path in root and extension.
// The extension is everything starting at the last dot in the last
// pathname component; the root is everything before that.
// It is always true that root + ext == p.
//
// Generic implementation of splitext, to be parametrized with
// the separators.
extern void _splitText(const std::string& p, char sep, char altsep, char extsep, std::string& rootOut, std::string& extOut);

__PATH_INTERNAL_END__


__PATH_BEGIN__

// Split a path in root and extension.
// The extension is everything starting at the last dot in the last
// pathname component; the root is everything before that.
// It is always true that root + ext == p.
extern void splitText(const std::string& fullPath, std::string& rootOut, std::string& extOut);

__PATH_END__


#endif // WASP_UTILS_PATH_H
