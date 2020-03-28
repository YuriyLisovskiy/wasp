/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

#include <string>

#include <gtest/gtest.h>

#include "../../../../src/core/logger.h"
#include "../../../../src/utils/crypto/hmac.h"

using namespace wasp;


TEST(HmacTestCase, Md5HasherTest)
{
	std::string key = "+s6cv712&nw4gsk)1dmgpje+f#%^4lhp@!up+=p3ts+hxz(fr2";
	auto hmac = utils::crypto::Hmac(key);

	ASSERT_EQ(hmac.size(), 16);
	ASSERT_EQ(hmac.block_size(), 64);

	hmac.update("The quick brown fox jumps over the lazy dog");
	ASSERT_EQ("811a406877bbee0e26be8ec53c6bcdf4", hmac.hex_digest());
}
