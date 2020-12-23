/**
 * http/tests_utility.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/http/utility.h"

using namespace xw;


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
