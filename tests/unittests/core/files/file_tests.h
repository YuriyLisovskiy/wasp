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

#ifndef WASP_FILE_TESTS_H
#define WASP_FILE_TESTS_H

#include <vector>
#include <string>
#include <cstdio>
#include <fstream>

#include <gtest/gtest.h>

#include "../../globals.h"
#include "../../../../src/utils/path.h"
#include "../../../../src/core/files/file.h"

using namespace wasp;


std::vector<byte> strToBytes(const std::string& s)
{
	return std::vector<byte>(s.cbegin(), s.cend());
}

std::string bytesToStr(const std::vector<byte>& bytes)
{
	return std::string(bytes.begin(), bytes.end());
}

void clear(const std::string& path)
{
	if (std::remove(path.c_str()) != 0)
	{
		print("Unable to remove file: \"" + path + "\"");
		ASSERT_TRUE(false);
	}
}

void createTestFile(const std::string& path, const std::vector<byte>& data)
{
	std::fstream f(path, std::ios::out);
	if (f.is_open())
	{
		f.write((char*) data.data(), data.size());
		f.close();
	}
	else
	{
		print("Unable to create file: \"" + path + "\"");
		ASSERT_TRUE(false);
	}
}

void assertFileContent(const std::string& fp, const std::vector<byte>& data, bool bin)
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

TEST(FileTestCase, TestReadMode)
{
	std::string filePath = path::cwd() + "/TestReadMode.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		auto actual = bytesToStr(file.read(-1));
		file.close();
		clear(filePath);

		ASSERT_EQ(expected, actual);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestReadString)
{
	std::string filePath = path::cwd() + "/TestReadString.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		auto actual = file.readStr();
		file.close();
		clear(filePath);

		ASSERT_EQ(expected, actual);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestReadBinaryMode)
{
	std::string filePath = path::cwd() + "/TestReadBinaryMode.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "rb");
	file.open();
	if (file.isOpen())
	{
		auto actual = bytesToStr(file.read(-1));
		file.close();
		clear(filePath);

		ASSERT_EQ(expected, actual);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestWriteMode)
{
	std::string filePath = path::cwd() + "/TestWriteMode.txt";
	File file(filePath, "w");
	file.open();
	if (file.isOpen())
	{
		std::vector<byte> byteArray = strToBytes("hello, world");
		file.write(byteArray);
		file.close();
		assertFileContent(filePath, byteArray, false);
		clear(filePath);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestWriteString)
{
	std::string filePath = path::cwd() + "/TestWriteString.txt";
	std::string expected = "hello, world";
	File file(filePath, "w");
	file.open();
	if (file.isOpen())
	{
		file.writeStr(expected);
		file.close();
		assertFileContent(filePath, strToBytes(expected), false);
		clear(filePath);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestWriteBinaryMode)
{
	std::string filePath = path::cwd() + "/TestWriteBinaryMode.txt";
	File file(filePath, "wb");
	file.open();
	if (file.isOpen())
	{
		std::vector<byte> byteArray = strToBytes("hello, world");
		file.write(byteArray);
		file.close();
		assertFileContent(filePath, byteArray, true);
		clear(filePath);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestMultipleChunksFalse)
{
	std::string filePath = path::cwd() + "/TestMultipleChunksFalse.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		ASSERT_FALSE(file.multipleChunks(-1));
		file.close();
		clear(filePath);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestMultipleChunksTrue)
{
	std::string filePath = path::cwd() + "/TestMultipleChunksTrue.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		ASSERT_TRUE(file.multipleChunks(4));
		file.close();
		clear(filePath);
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestReadFileIsNotOpenError)
{
	std::string filePath = path::cwd() + "/TestReadFileIsNotOpenError.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");

	ASSERT_THROW(file.read(-1), FileError);

	clear(filePath);
}

TEST(FileTestCase, TestReadFileIsInWriteOnlyModeError)
{
	std::string filePath = path::cwd() + "/TestReadFileIsInWriteOnlyModeError.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "w");
	file.open();

	ASSERT_TRUE(file.isOpen());
	ASSERT_THROW(file.read(-1), FileError);

	file.close();
	clear(filePath);
}

TEST(FileTestCase, TestWriteFileIsNotOpenError)
{
	std::string filePath = path::cwd() + "/TestWriteFileIsNotOpenError.txt";
	File file(filePath, "r");

	ASSERT_THROW(file.write(strToBytes("Hello, World")), FileError);
}

TEST(FileTestCase, TestWriteFileIsInReadOnlyModeError)
{
	std::string filePath = path::cwd() + "/TestWriteFileIsInReadOnlyModeError.txt";
	std::string expected = "Hello, World";
	createTestFile(filePath, strToBytes(expected));
	File file(filePath, "r");
	file.open();

	ASSERT_TRUE(file.isOpen());
	ASSERT_THROW(file.write(strToBytes("Hello, World")), FileError);

	file.close();
	clear(filePath);
}

TEST(FileTestCase, TestGetPath)
{
	std::string filePath = path::cwd() + "/TestGetPath.txt";
	File file(filePath, "w");

	ASSERT_EQ(file.path(), filePath);
}

TEST(FileTestCase, TestReadChunksDefault)
{
	std::string filePath = path::cwd() + "/TestReadChunksDefault.txt";
	std::vector<std::vector<byte>> expected = {
		strToBytes("Hello, World")
	};
	createTestFile(filePath, strToBytes("Hello, World"));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		auto actual = file.chunks();
		file.close();
		clear(filePath);

		ASSERT_EQ(expected.size(), actual.size());
		ASSERT_EQ(actual.size(), 1);

		for (size_t i = 0; i < expected[0].size(); i++)
		{
			ASSERT_EQ(expected[0][i], actual[0][i]);
		}
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

TEST(FileTestCase, TestReadChunksCustomized)
{
	std::string filePath = path::cwd() + "/TestReadChunksCustomized.txt";
	std::vector<std::vector<byte>> expected = {
		strToBytes("He"),
		strToBytes("ll"),
		strToBytes("o,"),
		strToBytes(" W"),
		strToBytes("or"),
		strToBytes("ld"),
	};
	createTestFile(filePath, strToBytes("Hello, World"));
	File file(filePath, "r");
	file.open();
	if (file.isOpen())
	{
		auto actual = file.chunks(2);
		file.close();
		clear(filePath);

		ASSERT_EQ(expected.size(), actual.size());

		for (size_t i = 0; i < expected.size(); i++)
		{
			ASSERT_EQ(actual[i][0], expected[i][0]);
			ASSERT_EQ(actual[i][1], expected[i][1]);
		}
	}
	else
	{
		print("File is not open");
		ASSERT_TRUE(false);
	}
}

#endif // WASP_FILE_TESTS_H
