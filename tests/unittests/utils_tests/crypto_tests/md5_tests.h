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

#ifndef WASP_UNIT_TESTS_UTILS_TESTS_CRYPTO_TESTS_MD5_TESTS_H
#define WASP_UNIT_TESTS_UTILS_TESTS_CRYPTO_TESTS_MD5_TESTS_H

#include <gtest/gtest.h>

#include "../../_def_.h"

#include "../../../../src/utils/crypto/md5.h"


__UNIT_TESTS_BEGIN__

TEST(Md5TestCase, TestDigest)
{
	auto expected = "9e107d9d372bb6826bd81d3542a419d6";
	auto actual = utils::crypto::md5("The quick brown fox jumps over the lazy dog").digest();
	ASSERT_EQ(actual, expected);

	expected = "e4d909c290d0fb1ca068ffaddf22cbd0";
	actual = utils::crypto::md5("The quick brown fox jumps over the lazy dog.").digest();
	ASSERT_EQ(actual, expected);

	expected = "d41d8cd98f00b204e9800998ecf8427e";
	actual = utils::crypto::md5("").digest();
	ASSERT_EQ(actual, expected);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILS_TESTS_CRYPTO_TESTS_MD5_TESTS_H
