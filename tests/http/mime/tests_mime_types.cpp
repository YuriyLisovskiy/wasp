/**
 * core/tests_mime_types.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/http/mime/media_type.h"

using namespace xw;


// Testing 'ext_from_file_name' function.
TEST(MimeTypesTestCase, extFromFileNameTestSuccess)
{
	ASSERT_EQ(http::mime::ext_from_file_name("foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_file_name(".bar"), "bar");
}

TEST(MimeTypesTestCase, extFromFileNameTestNoExtension)
{
	ASSERT_EQ(http::mime::ext_from_file_name("foo"), "");
	ASSERT_EQ(http::mime::ext_from_file_name("bar"), "");
}

TEST(MimeTypesTestCase, extFromFileNameTestEmptyExtension)
{
	ASSERT_EQ(http::mime::ext_from_file_name("foo."), "");
	ASSERT_EQ(http::mime::ext_from_file_name("bar."), "");
}

// Testing 'extFromPath' function.
TEST(MimeTypesTestCase, extFromPathTestSuccessWithSlashes)
{
	ASSERT_EQ(http::mime::ext_from_path("path/to/foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_path("/foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_path("./foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_path("./.foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestSuccessWithoutSlashes)
{
	ASSERT_EQ(http::mime::ext_from_path("foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_path(".foo.bar"), "bar");
	ASSERT_EQ(http::mime::ext_from_path("....foo.bar"), "bar");
}

TEST(MimeTypesTestCase, extFromPathTestNoExtension)
{
	ASSERT_EQ(http::mime::ext_from_path("foo"), "");
	ASSERT_EQ(http::mime::ext_from_path("path/to/foo"), "");
	ASSERT_EQ(http::mime::ext_from_path("/foo"), "");
	ASSERT_EQ(http::mime::ext_from_path("./foo"), "");
}

TEST(MimeTypesTestCase, extFromPathTestEmptyExtension)
{
	ASSERT_EQ(http::mime::ext_from_path("foo."), "");
	ASSERT_EQ(http::mime::ext_from_path("path/to/foo."), "");
	ASSERT_EQ(http::mime::ext_from_path("/foo."), "");
	ASSERT_EQ(http::mime::ext_from_path("./foo."), "");
	ASSERT_EQ(http::mime::ext_from_path("./.foo."), "");
	ASSERT_EQ(http::mime::ext_from_path("../.foo."), "");
}

// Testing 'guessContentType' function.
TEST(MimeTypesTestCase, guessContentTypeTestAll)
{
	std::string type, encoding;

	for (auto it = http::mime::TYPES_MAP.begin(); it != http::mime::TYPES_MAP.end(); it++)
	{
		http::mime::guess_content_type("./file" + it->first, type, encoding);
		ASSERT_EQ(type, it->second);
	}
}

TEST(MimeTypesTestCase, guessContentTypeTestTextSVGZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.svgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTGZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.tgz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTAZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.taz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.tz", type, encoding);
	ASSERT_EQ(type, "application/gzip");
	ASSERT_EQ(encoding, "gzip");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTBZ2)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.tbz2", type, encoding);
	ASSERT_EQ(type, "application/octet-stream");
	ASSERT_EQ(encoding, "bzip2");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextTXZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.txz", type, encoding);
	ASSERT_EQ(type, "application/x-xz");
	ASSERT_EQ(encoding, "xz");
}

TEST(MimeTypesTestCase, guessContentTypeTestTextZ)
{
	std::string type, encoding;

	http::mime::guess_content_type("path/to/backup.Z", type, encoding);
	ASSERT_EQ(type, "application/x-z");
	ASSERT_EQ(encoding, "compress");
}

TEST(MimeTypesTestCase, _consume_value)
{
	std::vector<std::vector<std::string>> data = {
		{"foo bar", "foo", " bar"},
		{"bar", "bar", ""},
		{" bar ", "", " bar "},
		{"\"My value\"end", "My value", "end"},
		{"\"My value\" end", "My value", " end"},
		{R"("\\" rest)", "\\", " rest"},
		{R"("My \" value"end)", "My \" value", "end"},
		{R"("\" rest)", "", R"("\" rest)"},
		{R"("C:\dev\go\robots.txt")", R"(C:\dev\go\robots.txt)", ""},
		{"\"C:\\新建文件件\\中文第二次测试.mp4\"", "C:\\新建文件件\\中文第二次测试.mp4", ""},
	};
	for (const auto& test : data)
	{
		auto [value, rest] = http::mime::_consume_value(str::string_to_wstring(test[0]));
		ASSERT_EQ(value, str::string_to_wstring(test[1]));
		ASSERT_EQ(rest, str::string_to_wstring(test[2]));
	}
}

TEST(MimeTypesTestCase, _consume_media_parameter)
{
	std::vector<std::vector<std::string>> data = {
		{" ; foo=bar", "foo", "bar", ""},
		{"; foo=bar", "foo", "bar", ""},
		{";foo=bar", "foo", "bar", ""},
		{";FOO=bar", "foo", "bar", ""},
		{R"(;foo="bar")", "foo", "bar", ""},
		{R"(;foo="bar"; )", "foo", "bar", "; "},
		{R"(;foo="bar"; foo=baz)", "foo", "bar", "; foo=baz"},
		{R"( ; boundary=----CUT;)", "boundary", "----CUT", ";"},
		{R"( ; key=value;  blah="value";name="foo" )", "key", "value", R"(;  blah="value";name="foo" )"},
		{R"(;  blah="value";name="foo" )", "blah", "value", R"(;name="foo" )"},
		{R"(;name="foo" )", "name", "foo", R"( )"},
	};
	size_t i = 0;
	for (const auto& test : data)
	{
		auto [param, value, rest] = http::mime::_consume_media_parameter(str::string_to_wstring(test[0]));
		ASSERT_EQ(param, str::string_to_wstring(test[1]));
		ASSERT_EQ(value, str::string_to_wstring(test[2]));
		ASSERT_EQ(rest, str::string_to_wstring(test[3]));
	}
}
