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

#ifndef WASP_CORE_FILES_FILE_H
#define WASP_CORE_FILES_FILE_H

#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#include "../../globals.h"
#include "../exceptions.h"
#include "../../utility/str.h"


__WASP_BEGIN__

class File
{
protected:
	enum FileMode
	{
		ReadOnly, WriteOnly, Both
	};

	size_t _defaultChunkSize;
	std::fstream _file;
	std::string _name;
	FileMode _fileMode;
	std::string _strMode;
	std::ios_base::openmode _mode;

	void _initMode(const std::string& mode);
	void seek(size_t n, std::ios_base::seekdir seekDir);
	void seek(size_t n);
	size_t tell();

public:
	explicit File(
		const std::string& name,
		const std::string& mode = "r"
	);
	File(
		const std::vector<byte>& data,
		const std::string& name,
		const std::string& mode = "wb"
	);

	void open();
	void save();
	void close();
	bool isOpen();
	std::vector<byte> read(size_t n = -1);
	std::string readStr(size_t n = -1);
	void write(std::vector<byte> bytes);
	void writeStr(const std::string& str);
	size_t size();
	std::vector<std::vector<byte>> chunks(size_t chunkSize = -1);
	bool multipleChunks(size_t chunkSize = -1);
	std::string path() const;
};

__WASP_END__


#endif // WASP_CORE_FILES_FILE_H
