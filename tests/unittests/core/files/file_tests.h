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

#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cstdio>
#include <fstream>

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
		char* buffer = new char[data.size()];
		f.read(buffer, data.size());
		for (size_t i = 0; i < data.size(); i++)
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
