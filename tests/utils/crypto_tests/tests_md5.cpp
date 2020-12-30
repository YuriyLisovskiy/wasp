/**
 * utils/crypto/tests_md5.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/utils/crypto/md5.h"

using namespace xw;


TEST(Md5TestCase, TestDigest)
{
	auto expected = "9e107d9d372bb6826bd81d3542a419d6";
	auto actual = utils::crypto::MD5("The quick brown fox jumps over the lazy dog").hex_digest();
	ASSERT_EQ(actual, expected);

	expected = "e4d909c290d0fb1ca068ffaddf22cbd0";
	actual = utils::crypto::MD5("The quick brown fox jumps over the lazy dog.").hex_digest();
	ASSERT_EQ(actual, expected);

	expected = "d41d8cd98f00b204e9800998ecf8427e";
	actual = utils::crypto::MD5("").hex_digest();
	ASSERT_EQ(actual, expected);
}
