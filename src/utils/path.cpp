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
 * path implementation.
 * TODO: write docs.
 */

#include "path.h"


__PATH_INTERNAL_BEGIN__

void _splitText(const std::string& fullPath, char sep, char altsep, char extsep, std::string& rootOut, std::string& extOut)
{
	rootOut = fullPath;
	extOut = "";
	size_t sepIdx = fullPath.rfind(sep);
	if (altsep != '\0')
	{
		size_t altsepIdx = fullPath.rfind(altsep);
		sepIdx = std::max(sepIdx, altsepIdx);
	}
	size_t dotIdx = fullPath.rfind(extsep);
	if (dotIdx > sepIdx)
	{
		// skip all leading dots
		size_t fileNameIdx = sepIdx + 1;
		while (fileNameIdx < dotIdx)
		{
			if (fullPath.at(fileNameIdx) != extsep)
			{
				rootOut = fullPath.substr(0, dotIdx);
				extOut = fullPath.substr(dotIdx);
				break;
			}
			fileNameIdx++;
		}
	}
}

__PATH_INTERNAL_END__


__PATH_BEGIN__

void splitText(const std::string& fullPath, std::string& rootOut, std::string& extOut)
{
	internal::_splitText(fullPath, '/', '\0', '.', rootOut, extOut);
}

bool exists(const std::string& path)
{
	return access(path.c_str(), 0) == 0;
}

std::string baseName(const std::string& path)
{
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
	{
		pos = 0;
	}
	return std::string(path.begin() + pos, path.end());
}

size_t getSize(const std::string& path)
{
	if (!exists(path))
	{
		throw FileDoesNotExistError("file '" + path + "' does not exist", _ERROR_DETAILS_);
	}
	return std::ifstream(path, std::ifstream::ate | std::ifstream::binary).tellg();
}

__PATH_END__
