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

#ifndef WASP_UNIT_TESTS_UTILS_ENCODING_TESTS_H
#define WASP_UNIT_TESTS_UTILS_ENCODING_TESTS_H

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/utils/encoding.h"


__UNIT_TESTS_BEGIN__

TEST(EncodeUrlTestCase, encode)
{
	std::string expected("https://hostName/deepLinkAction.do?userName=peter%40nable.com&password=Hello%25There&method=defaultDashboard");
	std::string toEncode("https://hostName/deepLinkAction.do?userName=peter@nable.com&password=Hello%There&method=defaultDashboard");
	ASSERT_EQ(wasp::encoding::encodeUrl(toEncode), expected);

	expected = "https://www.google.com/?q=%E3%83%95%E3%82%A9%E3%83%BC%E3%83%87%E3%82%A3%E3%83%BC";
	toEncode = "https://www.google.com/?q=フォーディー";
	ASSERT_EQ(wasp::encoding::encodeUrl(toEncode), expected);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_UTILS_ENCODING_TESTS_H
