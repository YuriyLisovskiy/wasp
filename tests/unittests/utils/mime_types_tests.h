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

#ifndef WASP_UNIT_TESTS_MIME_TYPES_TESTS_H
#define WASP_UNIT_TESTS_MIME_TYPES_TESTS_H

#include <gtest/gtest.h>

#include "../../../src/utils/mime_types.h"


// Testing 'extFromFileName' function.
TEST(MimeTypesTest, extFromFileNameTestSuccess)
{
	ASSERT_EQ(wasp::mime::extFromFileName("foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromFileName(".bar"), "bar");
}

TEST(MimeTypesTest, extFromFileNameTestNoExtension)
{
	ASSERT_EQ(wasp::mime::extFromFileName("foo"), "");
	ASSERT_EQ(wasp::mime::extFromFileName("bar"), "");
}

TEST(MimeTypesTest, extFromFileNameTestEmptyExtension)
{
	ASSERT_EQ(wasp::mime::extFromFileName("foo."), "");
	ASSERT_EQ(wasp::mime::extFromFileName("bar."), "");
}

// Testing 'extFromPath' function.
TEST(MimeTypesTest, extFromPathTestSuccessWithSlashes)
{
	ASSERT_EQ(wasp::mime::extFromPath("path/to/foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromPath("/foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromPath("./foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromPath("./.foo.bar"), "bar");
}

TEST(MimeTypesTest, extFromPathTestSuccessWithoutSlashes)
{
	ASSERT_EQ(wasp::mime::extFromPath("foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromPath(".foo.bar"), "bar");
	ASSERT_EQ(wasp::mime::extFromPath("....foo.bar"), "bar");
}

TEST(MimeTypesTest, extFromPathTestNoExtension)
{
	ASSERT_EQ(wasp::mime::extFromPath("foo"), "");
	ASSERT_EQ(wasp::mime::extFromPath("path/to/foo"), "");
	ASSERT_EQ(wasp::mime::extFromPath("/foo"), "");
	ASSERT_EQ(wasp::mime::extFromPath("./foo"), "");
}

TEST(MimeTypesTest, extFromPathTestEmptyExtension)
{
	ASSERT_EQ(wasp::mime::extFromPath("foo."), "");
	ASSERT_EQ(wasp::mime::extFromPath("path/to/foo."), "");
	ASSERT_EQ(wasp::mime::extFromPath("/foo."), "");
	ASSERT_EQ(wasp::mime::extFromPath("./foo."), "");
	ASSERT_EQ(wasp::mime::extFromPath("./.foo."), "");
	ASSERT_EQ(wasp::mime::extFromPath("../.foo."), "");
}

// Testing 'guessContentType' function.
TEST(MimeTypesTest, guessContentTypeTestAll)
{
	std::string type, encoding;

	for (auto it = wasp::mime::TYPES_MAP.cbegin(); it != wasp::mime::TYPES_MAP.cend(); it++)
	{
		wasp::mime::guessContentType("./file" + it->first, type, encoding);
		ASSERT_EQ(type, it->second);
	}
}

TEST(MimeTypesTest, guessContentTypeTestTextSVGZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.svgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTest, guessContentTypeTestTextTGZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.tgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTest, guessContentTypeTestTextTAZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.taz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTest, guessContentTypeTestTextTZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.tz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTest, guessContentTypeTestTextTBZ2)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.tbz2", type, encoding);
	ASSERT_EQ(type, "application/octet-stream");
	ASSERT_EQ(encoding, "bzip2");
}

TEST(MimeTypesTest, guessContentTypeTestTextTXZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.txz", type, encoding);
	ASSERT_EQ(type, "application/x-xz");
	ASSERT_EQ(encoding, "xz");
}

TEST(MimeTypesTest, guessContentTypeTestTextZ)
{
	std::string type, encoding;

	wasp::mime::guessContentType("path/to/backup.Z", type, encoding);
	ASSERT_EQ(type, "application/x-z");
	ASSERT_EQ(encoding, "compress");
}

#endif // WASP_UNIT_TESTS_MIME_TYPES_TESTS_H
