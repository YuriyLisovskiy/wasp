/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
 * An implementation of core/files/file.h
 */

#include "./file.h"


__CORE_BEGIN__

// Public members
File::File(const std::string& name, const std::string& mode)
{
	this->_default_chunk_size = 64 * std::pow(2, 10);
	this->_file = std::fstream();
	this->_name = name;
	this->_str_mode = mode;
	this->_init_mode(mode);
}

File::File(const std::vector<byte>& data, const std::string& name, const std::string& mode) : File(name, mode)
{
	this->_data = data;
}

File::File(const File& other)
{
	if (this != &other)
	{
		this->_default_chunk_size = other._default_chunk_size;
		this->_file = std::fstream();
		this->_name = other._name;
		this->_str_mode = other._str_mode;
		this->_data = other._data;
		this->_init_mode(other._str_mode);
	}
}

File& File::operator=(const File& other)
{
	if (this != &other)
	{
		this->_default_chunk_size = other._default_chunk_size;
		this->_file = std::fstream();
		this->_name = other._name;
		this->_str_mode = other._str_mode;
		this->_data = other._data;
		this->_init_mode(other._str_mode);
	}

	return *this;
}

void File::open()
{
	if (this->_name.empty())
	{
		throw FileError("unable to open file: path is empty", _ERROR_DETAILS_);
	}

	if (this->_file.is_open())
	{
		this->_file.seekg(0);
	}
	else
	{
		this->_file.open(this->_name, this->_mode);
	}
}

void File::save()
{
	if (this->_name.empty())
	{
		throw FileError("unable to save file: path is empty", _ERROR_DETAILS_);
	}

	this->_file.flush();
}

void File::save_file()
{
	if (this->_name.empty())
	{
		throw FileError("unable to save file: path is empty", _ERROR_DETAILS_);
	}

	if (this->_data.empty())
	{
		return;
	}

	this->open();
	this->write(this->_data);
	this->_data.clear();
	this->close();
}

void File::close()
{
	if (this->_name.empty())
	{
		throw FileError("unable to close file: path is empty", _ERROR_DETAILS_);
	}

	if (this->_file.is_open())
	{
		this->_file.close();
	}
}

bool File::is_open()
{
	if (this->_name.empty())
	{
		throw FileError("unable to check if file is open: path is empty", _ERROR_DETAILS_);
	}

	return this->_file.is_open();
}

std::vector<byte> File::read(size_t n)
{
	if (!this->_file.is_open())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	if (this->_file_mode == file_mode_enum::m_write_only)
	{
		throw FileError("file is open only for writing: " + this->_name, _ERROR_DETAILS_);
	}

	size_t actual_size = this->size() - this->tell();
	if (n < 0 || n > actual_size)
	{
		n = actual_size;
	}

	std::vector<byte> bytes;
	if (n > 0)
	{
		char* buffer = new char[n];
		this->_file.read(buffer, n);

		for (size_t i = 0; i < n; i++)
		{
			bytes.push_back(buffer[i]);
		}

		delete[] buffer;
	}

	return bytes;
}

std::string File::read_str(size_t n)
{
	std::vector<byte> bytes = this->read(n);
	return std::string(bytes.begin(), bytes.end());
}

void File::write(std::vector<byte> bytes)
{
	if (!this->_file.is_open())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	if (this->_file_mode == file_mode_enum::m_read_only)
	{
		throw FileError("file is open only for reading: " + this->_name, _ERROR_DETAILS_);
	}

	this->_file.write((char*) bytes.data(), bytes.size());
}

void File::write_str(const std::string& str)
{
	this->write(std::vector<byte>(str.begin(), str.end()));
}

size_t File::size()
{
	if (!this->is_open())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	size_t current_pos = this->tell();
	this->seek(0, std::fstream::end);
	size_t file_size = this->tell();
	this->seek(current_pos);
	return file_size;
}

std::vector<std::vector<byte>> File::chunks(size_t chunk_size)
{
	if (this->_name.empty())
	{
		throw FileError("unable to read file chunks: path is empty", _ERROR_DETAILS_);
	}

	if (chunk_size < 1)
	{
		chunk_size = this->_default_chunk_size;
	}

	this->seek(0);
	std::vector<std::vector<byte>> result;
	while (true)
	{
		std::vector<byte> chunk = this->read(chunk_size);
		if (chunk.empty())
		{
			break;
		}

		result.push_back(chunk);
	}

	return result;
}

bool File::multiple_chunks(size_t chunk_size)
{
	if (this->_name.empty())
	{
		throw FileError("unable to check multiple chunks: path is empty", _ERROR_DETAILS_);
	}

	if (chunk_size < 1)
	{
		chunk_size = this->_default_chunk_size;
	}

	return this->size() > chunk_size;
}

std::string File::path() const
{
	return this->_name;
}

// Private members
void File::_init_mode(const std::string& mode)
{
	if (mode == "r")
	{
		this->_mode = std::ios::in;
		this->_file_mode = file_mode_enum::m_read_only;
	}
	else if (mode == "rb")
	{
		this->_file_mode = file_mode_enum::m_read_only;
		this->_mode = std::ios::in | std::ios::binary;
	}
	else if (mode == "r+" || mode == "w+")
	{
		this->_file_mode = file_mode_enum::m_both;
		this->_mode = std::ios::out | std::ios::in;
	}
	else if (mode == "rb+" || mode == "wb+")
	{
		this->_file_mode = file_mode_enum::m_both;
		this->_mode = std::ios::out | std::ios::in | std::ios::binary;
	}
	else if (mode == "w")
	{
		this->_file_mode = file_mode_enum::m_write_only;
		this->_mode = std::ios::out;
	}
	else if (mode == "wb")
	{
		this->_file_mode = file_mode_enum::m_write_only;
		this->_mode = std::ios::out | std::ios::binary;
	}
	else
	{
		throw FileError("invalid file mode: " + this->_name, _ERROR_DETAILS_);
	}
}

void File::seek(size_t n, std::ios_base::seekdir seek_dir)
{
	if (this->_name.empty())
	{
		throw FileError("unable to seek: path is empty", _ERROR_DETAILS_);
	}

	if (this->_file_mode == file_mode_enum::m_read_only)
	{
		this->_file.seekg(n, seek_dir);
	}
	else
	{
		this->_file.seekp(n, seek_dir);
	}
}

void File::seek(size_t n)
{
	if (this->_name.empty())
	{
		throw FileError("unable to seek: path is empty", _ERROR_DETAILS_);
	}

	if (this->_file_mode == file_mode_enum::m_read_only)
	{
		this->_file.seekg(n);
	}
	else
	{
		this->_file.seekp(n);
	}
}

size_t File::tell()
{
	if (this->_name.empty())
	{
		throw FileError("unable to tell: path is empty", _ERROR_DETAILS_);
	}

	if (this->_file_mode == file_mode_enum::m_read_only)
	{
		return this->_file.tellg();
	}
	else
	{
		return this->_file.tellp();
	}
}

struct stat File::stat(const std::string& file_path)
{
	struct stat result{};
	::stat(file_path.c_str(), &result);
	return result;
}

__CORE_END__
