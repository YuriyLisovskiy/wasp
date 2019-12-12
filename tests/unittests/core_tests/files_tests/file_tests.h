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

#ifndef WASP_UNIT_TESTS_CORE_TESTS_FILES_TESTS_FILE_TESTS_H
#define WASP_UNIT_TESTS_CORE_TESTS_FILES_TESTS_FILE_TESTS_H

#include <vector>
#include <string>
#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "../../globals.h"
#include "../../../../src/utility/path.h"
#include "../../../../src/core/files/file.h"

using byte = wasp::byte;
using File = wasp::File;


std::vector<byte> strToBytes(const std::string& s)
{
	return std::vector<byte>(s.cbegin(), s.cend());
}

void removeFile(const std::string& path)
{
	if (std::remove(path.c_str()) != 0)
	{
		ASSERT_TRUE(false);
	}
}


class ReadFileTestCase : public ::testing::Test
{
protected:
	const std::string testReadFilePath = wasp::path::cwd() + "/TestReadFile.txt";
	File fileToRead = File(this->testReadFilePath, "r");

	void SetUp() override
	{
		createFile(this->testReadFilePath, strToBytes("Hello, World"));

		this->fileToRead.open();
		ASSERT_TRUE(this->fileToRead.isOpen());
	}

	void TearDown() override
	{
		this->fileToRead.close();
		removeFile(this->testReadFilePath);
	}

	static std::string bytesToStr(const std::vector<byte>& bytes)
	{
		return std::string(bytes.begin(), bytes.end());
	}

	static void createFile(const std::string& path, const std::vector<byte>& data)
	{
		std::fstream f(path, std::ios::out);
		if (f.is_open())
		{
			f.write((char*) data.data(), data.size());
			f.close();
		}
		else
		{
			ASSERT_TRUE(false);
		}
	}
};

TEST_F(ReadFileTestCase, TestReadMode)
{
	std::string expected = "Hello, World";
	auto actual = bytesToStr(this->fileToRead.read());

	ASSERT_EQ(expected, actual);
}

TEST_F(ReadFileTestCase, TestReadString)
{
	std::string expected = "Hello, World";
	auto actual = this->fileToRead.readStr();

	ASSERT_EQ(expected, actual);
}

TEST_F(ReadFileTestCase, TestReadBinaryMode)
{
	File f(this->testReadFilePath, "rb");
	f.open();

	ASSERT_TRUE(f.isOpen());

	std::string expected = "Hello, World";
	std::string actual = bytesToStr(f.read());
	f.close();

	ASSERT_EQ(expected, actual);
}

TEST_F(ReadFileTestCase, TestReadFileIsNotOpenError)
{
	this->fileToRead.close();

	ASSERT_THROW(this->fileToRead.read(), wasp::FileError);

	this->fileToRead.open();
}

TEST_F(ReadFileTestCase, TestWriteFileIsInReadOnlyModeError)
{
	ASSERT_THROW(this->fileToRead.write(strToBytes("Hello, World")), wasp::FileError);
}

TEST_F(ReadFileTestCase, TestGetPath)
{
	ASSERT_EQ(this->fileToRead.path(), this->testReadFilePath);
}

TEST_F(ReadFileTestCase, TestReadChunksDefault)
{
	std::vector<std::vector<byte>> expected = {
		strToBytes("Hello, World")
	};
	auto actual = this->fileToRead.chunks();

	ASSERT_EQ(expected.size(), actual.size());
	ASSERT_EQ(actual.size(), 1);

	for (size_t i = 0; i < expected[0].size(); i++)
	{
		ASSERT_EQ(expected[0][i], actual[0][i]);
	}
}

TEST_F(ReadFileTestCase, TestReadChunksCustomized)
{
	std::vector<std::vector<byte>> expected = {
		strToBytes("He"),
		strToBytes("ll"),
		strToBytes("o,"),
		strToBytes(" W"),
		strToBytes("or"),
		strToBytes("ld"),
	};
	auto actual = this->fileToRead.chunks(2);

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(actual[i][0], expected[i][0]);
		ASSERT_EQ(actual[i][1], expected[i][1]);
	}
}

TEST_F(ReadFileTestCase, TestMultipleChunksFalse)
{
	ASSERT_FALSE(this->fileToRead.multipleChunks());
}

TEST_F(ReadFileTestCase, TestMultipleChunksTrue)
{
	ASSERT_TRUE(this->fileToRead.multipleChunks(4));
}


class WriteFileTestCase : public ::testing::Test
{
protected:
	const std::string testFilePath = wasp::path::cwd() + "/TestWriteFile.txt";
	File file = File(this->testFilePath, "w");

	void SetUp() override
	{
		this->file.open();
		ASSERT_TRUE(this->file.isOpen());
	}

	void TearDown() override
	{
		this->file.close();
		removeFile(this->testFilePath);
	}

	static void assertFile(const std::string& fp, const std::vector<byte>& data, bool bin)
	{
		std::ios_base::openmode mode = std::ios::in;
		if (bin)
		{
			mode |= std::ios::binary;
		}

		std::fstream f(fp, mode);
		if (f.is_open())
		{
			size_t n = data.size();
			char* buffer = new char[n];
			f.read(buffer, n);
			buffer[n] = '\0';

			std::string actual(buffer);
			ASSERT_EQ(actual.size(), n);

			for (size_t i = 0; i < n; i++)
			{
				ASSERT_EQ(data[i], buffer[i]);
			}

			delete[] buffer;
		}
		else
		{
			ASSERT_TRUE(false);
		}
	}
};

TEST_F(WriteFileTestCase, TestWriteMode)
{
	std::vector<byte> byteArray = strToBytes("Hello, World");
	this->file.write(byteArray);
	this->file.save();

	WriteFileTestCase::assertFile(this->testFilePath, byteArray, false);
}

TEST_F(WriteFileTestCase, TestWriteString)
{
	std::string expected = "Hello, World";
	this->file.writeStr(expected);
	this->file.save();

	WriteFileTestCase::assertFile(this->testFilePath, strToBytes(expected), false);
}

TEST_F(WriteFileTestCase, TestWriteBinaryMode)
{
	File f(this->testFilePath, "wb");
	f.open();

	ASSERT_TRUE(f.isOpen());

	std::vector<byte> byteArray = strToBytes("Hello, World");
	f.write(byteArray);
	f.close();

	WriteFileTestCase::assertFile(this->testFilePath, byteArray, true);
}

TEST_F(WriteFileTestCase, TestWriteFileIsNotOpenError)
{
	this->file.close();

	ASSERT_THROW(this->file.write(strToBytes("Hello, World")), wasp::FileError);

	this->file.open();
}

TEST_F(WriteFileTestCase, TestReadFileIsInWriteOnlyModeError)
{
	ASSERT_THROW(file.read(), wasp::FileError);
}

#endif // WASP_UNIT_TESTS_CORE_TESTS_FILES_TESTS_FILE_TESTS_H
