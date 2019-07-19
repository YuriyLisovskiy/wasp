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

#ifndef WASP_UNIT_TESTS_HTTP_COOKIE_TESTS_H
#define WASP_UNIT_TESTS_HTTP_COOKIE_TESTS_H

#include <gtest/gtest.h>

#include "../../../src/globals.h"
#include "../../../src/http/cookie.h"


__INTERNAL_BEGIN__

TEST(CookieTest, toStringTestAllParameters)
{
	Cookie cookie("hello", "World", "Thu, 18 Jul 2019 16:25:19 GMT", "localhost.com", "/hello", true, true);
	std::string expected = "Set-Cookie: hello=World; Domain=localhost.com; Path=/hello; Expires=Thu, 18 Jul 2019 16:25:19 GMT; Secure; HttpOnly";
	ASSERT_EQ(cookie.toString(), expected);
}

TEST(CookieTest, toStringTestHttpOnlyIsFalse)
{
	Cookie cookie("hello", "1", "Thu, 18 Jul 2019 16:25:19 GMT", "localhost.com", "/hello", true, false);
	std::string expected = "Set-Cookie: hello=1; Domain=localhost.com; Path=/hello; Expires=Thu, 18 Jul 2019 16:25:19 GMT; Secure";
	ASSERT_EQ(cookie.toString(), expected);
}

TEST(CookieTest, toStringTestOnlyRequiredParameters)
{
	Cookie cookie("hello", "2.2", "Thu, 18 Jul 2019 16:25:19 GMT");
	std::string expected = "Set-Cookie: hello=2.2; Path=/; Expires=Thu, 18 Jul 2019 16:25:19 GMT; Secure";
	ASSERT_EQ(cookie.toString(), expected);
}

TEST(CookieTest, toStringTestEmptyPath)
{
	Cookie cookie("hello", "2.2", "Thu, 18 Jul 2019 16:25:19 GMT", "", "");
	std::string expected = "Set-Cookie: hello=2.2; Expires=Thu, 18 Jul 2019 16:25:19 GMT; Secure";
	ASSERT_EQ(cookie.toString(), expected);
}

__INTERNAL_END__


#endif // WASP_UNIT_TESTS_HTTP_COOKIE_TESTS_H
