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

#ifndef WASP_UNIT_TESTS_UTILITY_TESTS_MIME_TYPES_TESTS_H
#define WASP_UNIT_TESTS_UTILITY_TESTS_MIME_TYPES_TESTS_H

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/utility/mime_types.h"


__UNIT_TESTS_BEGIN__

// Testing 'extFromFileName' function.
TEST(MimeTypesTestCase, extFromFileNameTestSuccess)
{
	ASSERT_EQ(mime::ext_from_file_name("foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_file_name(".bar"), "bar");
}

TEST(MimeTypesTestCase, extFromFileNameTestNoExtension)
{
	ASSERT_EQ(mime::ext_from_file_name("foo"), "");
	ASSERT_EQ(mime::ext_from_file_name("bar"), "");
}

TEST(MimeTypesTestCase, extFromFileNameTestEmptyExtension)
{
	ASSERT_EQ(mime::ext_from_file_name("foo."), "");
	ASSERT_EQ(mime::ext_from_file_name("bar."), "");
}

// Testing 'extFromPath' function.
TEST(MimeTypesTestCase, extFromPathTestSuccessWithSlashes)
{
	ASSERT_EQ(mime::ext_from_path("path/to/foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_path("/foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_path("./foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_path("./.foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestSuccessWithoutSlashes)
{
	ASSERT_EQ(mime::ext_from_path("foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_path(".foo.bar"), "bar");
	ASSERT_EQ(mime::ext_from_path("....foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestNoExtension)
{
	ASSERT_EQ(mime::ext_from_path("foo"), "");
	ASSERT_EQ(mime::ext_from_path("path/to/foo"), "");
	ASSERT_EQ(mime::ext_from_path("/foo"), "");
	ASSERT_EQ(mime::ext_from_path("./foo"), "");
}

TEST(MimeTypesTestCase, extFromPathTestEmptyExtension)
{
	ASSERT_EQ(mime::ext_from_path("foo."), "");
	ASSERT_EQ(mime::ext_from_path("path/to/foo."), "");
	ASSERT_EQ(mime::ext_from_path("/foo."), "");
	ASSERT_EQ(mime::ext_from_path("./foo."), "");
	ASSERT_EQ(mime::ext_from_path("./.foo."), "");
	ASSERT_EQ(mime::ext_from_path("../.foo."), "");
}

// Testing 'guessContentType' function.
TEST(MimeTypesTestCase, guessContentTypeTestAll)
{
	std::string type, encoding;

	for (auto it = mime::TYPES_MAP.cbegin(); it != mime::TYPES_MAP.cend(); it++)
	{
		mime::guess_content_type("./file" + it->first, type, encoding);
		ASSERT_EQ(type, it->second);
	}
}

TEST(MimeTypesTestCase, guessContentTypeTestTextSVGZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.svgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTGZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.tgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTAZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.taz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.tz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTBZ2)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.tbz2", type, encoding);
	ASSERT_EQ(type, "application/octet-stream");
	ASSERT_EQ(encoding, "bzip2");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTXZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.txz", type, encoding);
	ASSERT_EQ(type, "application/x-xz");
	ASSERT_EQ(encoding, "xz");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextZ)
{
	std::string type, encoding;

	mime::guess_content_type("path/to/backup.Z", type, encoding);
	ASSERT_EQ(type, "application/x-z");
	ASSERT_EQ(encoding, "compress");
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILITY_TESTS_MIME_TYPES_TESTS_H
