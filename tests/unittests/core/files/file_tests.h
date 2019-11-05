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
	std::vector<byte> res;
	for (byte b : s)
	{
		res.push_back(b);
	}

	return res;
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

TEST(FileTestCase, TestWriteMode)
{
	std::string filePath = path::cwd() + "/test.txt";
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
	}
}

TEST(FileTestCase, TestWriteBinaryMode)
{
	std::string filePath = path::cwd() + "/test.txt";
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
	}
}


#endif // WASP_FILE_TESTS_H
