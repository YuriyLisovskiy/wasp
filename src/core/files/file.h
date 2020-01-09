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
 * file.h
 * Purpose: file stream wrapper for easier file handling.
 */

#pragma once

// C++ libraries.
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

#if defined(__unix__) || defined(__linux__)
#include <sys/types.h>
#include <sys/stat.h>
#endif

#ifndef _WIN32
#include <unistd.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
#define stat _stat
#endif

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../exceptions.h"
#include "../string/str.h"


__CORE_BEGIN__

typedef unsigned char byte;

class File
{
protected:
	enum file_mode_enum
	{
		m_read_only, m_write_only, m_both
	};

	size_t _default_chunk_size;
	std::fstream _file;
	std::string _name;
	file_mode_enum _file_mode;
	std::string _str_mode;
	std::ios_base::openmode _mode;
	std::vector<byte> _data;

	void _init_mode(const std::string& mode);
	void seek(size_t n, std::ios_base::seekdir seek_dir);
	void seek(size_t n);
	size_t tell();

public:
	explicit File(
		const std::string& name = "",
		const std::string& mode = "r"
	);

	File(
		const std::vector<byte>& data,
		const std::string& name,
		const std::string& mode = "wb"
	);

	File(const File& other);

	File& operator=(const File& other);

	void open();

	// Saves file without closing.
	void save();

	// Opens file if it is not opened, writes data
	//  if byte vector is not empty and closes file.
	void save_file();

	void close();
	bool is_open();
	std::vector<byte> read(size_t n = -1);
	std::string read_str(size_t n = -1);
	void write(std::vector<byte> bytes);
	void write_str(const std::string& str);
	size_t size();
	std::vector<std::vector<byte>> chunks(size_t chunk_size = -1);
	bool multiple_chunks(size_t chunk_size = -1);
	std::string path() const;

	static struct stat stat(const std::string& file_path);
};

__CORE_END__
