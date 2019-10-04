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

#include "file.h"


__WASP_BEGIN__

// Private members
void File::_initMode(const std::string& mode)
{
	if (mode == "r")
	{
		this->_mode = std::ios::in;
		this->_fileMode = FileMode::ReadOnly;
	}
	else if (mode == "rb")
	{
		this->_fileMode = FileMode::ReadOnly;
		this->_mode = std::ios::in | std::ios::binary;
	}
	else if (mode == "r+" || mode == "w+")
	{
		this->_fileMode = FileMode::Both;
		this->_mode = std::ios::out | std::ios::in;
	}
	else if (mode == "rb+" || mode == "wb+")
	{
		this->_fileMode = FileMode::Both;
		this->_mode = std::ios::out | std::ios::in | std::ios::binary;
	}
	else if (mode == "w")
	{
		this->_fileMode = FileMode::WriteOnly;
		this->_mode = std::ios::out;
	}
	else if (mode == "wb")
	{
		this->_fileMode = FileMode::WriteOnly;
		this->_mode = std::ios::out | std::ios::binary;
	}
	else
	{
		throw FileError("invalid file mode: " + this->_name, _ERROR_DETAILS_);
	}
}

void File::seek(size_t n, std::ios_base::seekdir seekDir)
{
	if (this->_fileMode == FileMode::ReadOnly)
	{
		this->_file.seekg(n, seekDir);
	}
	else
	{
		this->_file.seekp(n, seekDir);
	}
}

void File::seek(size_t n)
{
	if (this->_fileMode == FileMode::ReadOnly)
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
	if (this->_fileMode == FileMode::ReadOnly)
	{
		return this->_file.tellg();
	}
	else
	{
		return this->_file.tellp();
	}
}

// Public members
File::File(const std::string& name, const std::string& mode)
{
	this->_defaultChunkSize = 64 * std::pow(2, 10);
	this->_file = std::fstream();
	this->_name = name;
	this->_strMode = mode;
	this->_initMode(mode);
}

File::File(const std::vector<byte>& data, const std::string& name, const std::string& mode) : File(name, mode)
{
	this->open();
	this->write(data);
}

void File::open()
{
	if (this->_file.is_open())
	{
		this->_file.seekg(0);
	}
	else
	{
		this->_file.open(this->_name, this->_mode);
	}
}

// TODO: test this function
// TODO: throws segmentation fault
void File::close()
{
	if (this->_file.is_open())
	{
		this->_file.close();
	}
}

bool File::isOpen()
{
	return this->_file.is_open();
}

std::vector<byte> File::read(size_t n)
{
	if (!this->_file.is_open())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	if (this->_fileMode == FileMode::WriteOnly)
	{
		throw FileError("file is open only for writing: " + this->_name, _ERROR_DETAILS_);
	}

	std::vector<byte> bytes;
	char* buffer = new char;
	size_t i = 0;
	while (i != n && !this->_file.eof())
	{
		this->_file.read(buffer, 1);
		bytes.push_back((byte) *buffer);
		i++;
	}

	delete buffer;
	return bytes;
}

void File::write(std::vector<byte> bytes)
{
	if (!this->_file.is_open())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	if (this->_fileMode == FileMode::ReadOnly)
	{
		throw FileError("file is open only for reading: " + this->_name, _ERROR_DETAILS_);
	}

	this->_file.write((char*) bytes.data(), bytes.size() * sizeof(byte));
}

size_t File::size()
{
	if (!this->isOpen())
	{
		throw FileError("file is not open: " + this->_name, _ERROR_DETAILS_);
	}

	size_t currentPos = this->tell();
	this->seek(0, std::fstream::end);
	size_t fileSize = this->tell();
	this->seek(currentPos);
	return fileSize;
}

std::vector<std::vector<byte>> File::chunks(size_t chunkSize)
{
	if (chunkSize == 0)
	{
		chunkSize = this->_defaultChunkSize;
	}

	this->seek(0);
	std::vector<std::vector<byte>> result;
	while (true)
	{
		std::vector<byte> chunk = this->read(chunkSize);
		if (chunk.empty())
		{
			break;
		}

		result.push_back(chunk);
	}

	return result;
}

bool File::multipleChunks(size_t chunkSize)
{
	if (chunkSize == 0)
	{
		chunkSize = this->_defaultChunkSize;
	}

	return this->size() > chunkSize;
}

std::string File::path() const
{
	return this->_name;
}

__WASP_END__
