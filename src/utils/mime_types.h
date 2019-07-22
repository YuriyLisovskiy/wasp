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
 * mime_types definition.
 * TODO: write docs.
 */

#ifndef WASP_UTILS_MIME_TYPES_H
#define WASP_UTILS_MIME_TYPES_H

#include <string>

#include "../globals.h"


__MIME_BEGIN__

namespace internal
{

std::string getExtFromFileName(const std::string& fileName);

}

std::string getExtensionFromPath(const std::string& path);

std::string lookup(const std::string& str);

std::string contentType(const std::string& str);

std::string extension(const std::string& type);

std::string charset(const std::string& type);

__MIME_END__


#endif // WASP_UTILS_MIME_TYPES_H
