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

TEST(SplitDomainPort_TestCase, split_domain_port)
{
	std::string host = "127.0.0.1:8000";
	std::string domain, port;
	http::split_domain_port(host, domain, port);
	ASSERT_EQ(domain, "127.0.0.1");
	ASSERT_EQ(port, "8000");
}

TEST(ValidateHost_TestCase, validate_host)
{
	std::string domain = "127.0.0.1";
	std::vector<std::string> allowed_hosts = {".localhost", "127.0.0.1", "::1"};
	ASSERT_TRUE(http::validate_host(domain, allowed_hosts));
}
