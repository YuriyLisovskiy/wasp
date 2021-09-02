/**
 * core/tests_mime_types.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/core/media_type.h"

using namespace xw;


// Testing 'ext_from_file_name' function.
TEST(MimeTypesTestCase, extFromFileNameTestSuccess)
{
	ASSERT_EQ(core::mime::ext_from_file_name("foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_file_name(".bar"), "bar");
}

TEST(MimeTypesTestCase, extFromFileNameTestNoExtension)
{
	ASSERT_EQ(core::mime::ext_from_file_name("foo"), "");
	ASSERT_EQ(core::mime::ext_from_file_name("bar"), "");
}

TEST(MimeTypesTestCase, extFromFileNameTestEmptyExtension)
{
	ASSERT_EQ(core::mime::ext_from_file_name("foo."), "");
	ASSERT_EQ(core::mime::ext_from_file_name("bar."), "");
}

// Testing 'extFromPath' function.
TEST(MimeTypesTestCase, extFromPathTestSuccessWithSlashes)
{
	ASSERT_EQ(core::mime::ext_from_path("path/to/foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_path("/foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_path("./foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_path("./.foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestSuccessWithoutSlashes)
{
	ASSERT_EQ(core::mime::ext_from_path("foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_path(".foo.bar"), "bar");
	ASSERT_EQ(core::mime::ext_from_path("....foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestNoExtension)
{
	ASSERT_EQ(core::mime::ext_from_path("foo"), "");
	ASSERT_EQ(core::mime::ext_from_path("path/to/foo"), "");
	ASSERT_EQ(core::mime::ext_from_path("/foo"), "");
	ASSERT_EQ(core::mime::ext_from_path("./foo"), "");
}

TEST(MimeTypesTestCase, extFromPathTestEmptyExtension)
{
	ASSERT_EQ(core::mime::ext_from_path("foo."), "");
	ASSERT_EQ(core::mime::ext_from_path("path/to/foo."), "");
	ASSERT_EQ(core::mime::ext_from_path("/foo."), "");
	ASSERT_EQ(core::mime::ext_from_path("./foo."), "");
	ASSERT_EQ(core::mime::ext_from_path("./.foo."), "");
	ASSERT_EQ(core::mime::ext_from_path("../.foo."), "");
}

// Testing 'guessContentType' function.
TEST(MimeTypesTestCase, guessContentTypeTestAll)
{
	std::string type, encoding;

	for (auto it = core::mime::TYPES_MAP.begin(); it != core::mime::TYPES_MAP.end(); it++)
	{
		core::mime::guess_content_type("./file" + it->first, type, encoding);
		ASSERT_EQ(type, it->second);
	}
}

TEST(MimeTypesTestCase, guessContentTypeTestTextSVGZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.svgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTGZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.tgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTAZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.taz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.tz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTBZ2)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.tbz2", type, encoding);
	ASSERT_EQ(type, "application/octet-stream");
	ASSERT_EQ(encoding, "bzip2");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTXZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.txz", type, encoding);
	ASSERT_EQ(type, "application/x-xz");
	ASSERT_EQ(encoding, "xz");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextZ)
{
	std::string type, encoding;

	core::mime::guess_content_type("path/to/backup.Z", type, encoding);
	ASSERT_EQ(type, "application/x-z");
	ASSERT_EQ(encoding, "compress");
}
