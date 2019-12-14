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
 * An implementation of path.h.
 */

#include "path.h"


__PATH_INTERNAL_BEGIN__

void _split_text(
	const std::string& full_path,
	char sep,
	char altsep,
	char extsep,
	std::string& root_out,
	std::string& ext_out
)
{
	root_out = full_path;
	ext_out = "";
	size_t sep_idx = full_path.rfind(sep);
	if (altsep != '\0')
	{
		size_t altsep_idx = full_path.rfind(altsep);
		sep_idx = std::max(sep_idx, altsep_idx);
	}

	size_t dot_idx = full_path.rfind(extsep);
	if (dot_idx > sep_idx)
	{
		// skip all leading dots
		size_t file_name_idx = sep_idx + 1;
		while (file_name_idx < dot_idx)
		{
			if (full_path.at(file_name_idx) != extsep)
			{
				root_out = full_path.substr(0, dot_idx);
				ext_out = full_path.substr(dot_idx);
				break;
			}

			file_name_idx++;
		}
	}
}

__PATH_INTERNAL_END__


__PATH_BEGIN__

void split_text(const std::string& full_path, std::string& root_out, std::string& ext_out)
{
	internal::_split_text(full_path, '/', '\0', '.', root_out, ext_out);
}

bool exists(const std::string& path)
{
	return access(path.c_str(), 0) == 0;
}

std::string base(const std::string& path)
{
	size_t pos = path.rfind('/');
	if (pos == std::string::npos)
	{
		pos = 0;
	}

	return std::string(path.begin() + pos, path.end());
}

size_t get_size(const std::string& path)
{
	if (!exists(path))
	{
		throw FileDoesNotExistError("file '" + path + "' does not exist", _ERROR_DETAILS_);
	}

	std::ifstream ifs(path, std::ifstream::ate | std::ifstream::binary);
	size_t result = ifs.tellg();
	ifs.close();
	return result;
}

std::string cwd()
{
	char buffer[FILENAME_MAX];
	getcwd(buffer, FILENAME_MAX);
	return std::string(buffer);
}

__PATH_END__
