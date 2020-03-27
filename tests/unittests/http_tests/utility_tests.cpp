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

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/http/utility.h"


__UNIT_TESTS_BEGIN__

TEST(IsSameDomainTestCase, TestPatternIsEmpty)
{
	auto domain = ".localhost";
	ASSERT_FALSE(http::is_same_domain(domain, ""));
}

TEST(IsSameDomainTestCase, TestReturnsFalse)
{
	auto domain = ".local";
	auto pattern = ".localhost";
	ASSERT_FALSE(http::is_same_domain(domain, pattern));

	domain = "www.example.com";
	pattern = "example.com.ua";
	ASSERT_FALSE(http::is_same_domain(domain, pattern));
}

TEST(IsSameDomainTestCase, TestReturnsTrue)
{
	auto domain = ".localhost";
	auto pattern = ".localhost";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "example.com";
	pattern = ".example.com";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "127.0.0.1";
	pattern = "127.0.0.1";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));

	domain = "[::1]";
	pattern = "[::1]";
	ASSERT_TRUE(http::is_same_domain(domain, pattern));
}

__UNIT_TESTS_END__
