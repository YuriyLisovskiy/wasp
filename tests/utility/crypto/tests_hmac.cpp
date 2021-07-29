/**
 * utils/crypto/tests_hmac.cpp
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 */

#include <string>

#include <gtest/gtest.h>

#include "../../../src/utility/crypto/hmac.h"

using namespace xw;


TEST(HmacTestCase, Md5HasherTest)
{
	std::string key = "+s6cv712&nw4gsk)1dmgpje+f#%^4lhp@!up+=p3ts+hxz(fr2";
	auto hmac = util::crypto::HMAC(key);

	ASSERT_EQ(hmac.size(), 16);
	ASSERT_EQ(hmac.block_size(), 64);

	hmac.update("The quick brown fox jumps over the lazy dog");
	ASSERT_EQ("811a406877bbee0e26be8ec53c6bcdf4", hmac.hex_digest());
}
