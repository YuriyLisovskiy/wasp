/**
 * core/tests_encoding.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/core/encoding.h"

using namespace xw;


TEST(EncodeUrlTestCase, encode)
{
	std::string expected("https://hostName/deepLinkAction.do?userName=peter%40nable.com&password=Hello%25There&method=defaultDashboard");
	std::string toEncode("https://hostName/deepLinkAction.do?userName=peter@nable.com&password=Hello%There&method=defaultDashboard");
	ASSERT_EQ(core::encoding::encode_url(toEncode), expected);

	expected = "https://www.google.com/?q=%E3%83%95%E3%82%A9%E3%83%BC%E3%83%87%E3%82%A3%E3%83%BC";
	toEncode = "https://www.google.com/?q=フォーディー";
	ASSERT_EQ(core::encoding::encode_url(toEncode), expected);
}
