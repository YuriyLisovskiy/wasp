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
 * mime_types implementation.
 * TODO: write docs.
 */

#include "mime_types.h"


#define __MIME_INTERNAL_BEGIN__ __MIME_BEGIN__ namespace internal {
#define __MIME_INTERNAL_END__ } __MIME_END__


__MIME_INTERNAL_BEGIN__

std::string getExtFromFileName(const std::string& fileName)
{
	std::string result;
	size_t pos = fileName.find_last_of('.');
	if (pos != std::string::npos)
	{
		result = pos + 1 < fileName.size() ? fileName.substr(pos + 1) : "";
	}
	return result;
}

__MIME_INTERNAL_END__

__MIME_BEGIN__

std::string getExtensionFromPath(const std::string& path)
{
	std::string fileName;
	size_t pos = path.find_last_of('/');
	if (pos != std::string::npos)
	{
		fileName = pos + 1 < path.size() ? path.substr(pos + 1) : "";
	}
	else
	{
		pos = path.find_last_of('\\');
		if (pos != std::string::npos)
		{
			fileName = pos + 1 < path.size() ? path.substr(pos + 1) : "";
		}
		else
		{
			fileName = path;
		}
	}
	return fileName.empty() ? "" : internal::getExtFromFileName(fileName);
}

std::string lookup(const std::string& str)
{
	// TODO
	return "text/html";
}

std::string contentType(const std::string& str)
{
	// TODO
	return "text/html; charset=utf-8";
}

std::string extension(const std::string& type)
{
	// TODO
	return "html";
}

std::string charset(const std::string& type)
{
	// TODO
	return "utf-8";
}

__MIME_END__
