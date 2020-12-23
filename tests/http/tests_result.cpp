/**
 * http/tests_result.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../src/http/result.h"

using namespace xw;


TEST(FuncResult_TestCase, ConstructFromValue)
{
	auto result = http::Result<int>(2020);
	ASSERT_FALSE(result.err);
	ASSERT_EQ(result.value, 2020);
}

TEST(FuncResult_TestCase, RaiseException)
{
	using http::error_type;
	auto result = http::raise<error_type::HttpError, int>("an error");

	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::HttpError));

	ASSERT_FALSE(result.catch_(error_type::None));
	ASSERT_FALSE(result.catch_(error_type::EntityTooLargeError));
	ASSERT_FALSE(result.catch_(error_type::FileDoesNotExistError));
	ASSERT_FALSE(result.catch_(error_type::PermissionDenied));
	ASSERT_FALSE(result.catch_(error_type::SuspiciousOperation));
	ASSERT_FALSE(result.catch_(error_type::DisallowedHost));
	ASSERT_FALSE(result.catch_(error_type::DisallowedRedirect));
}

TEST(FuncResult_TestCase, RaiseEntityTooLargeError)
{
	using http::error_type;
	auto result = http::raise<error_type::EntityTooLargeError, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::EntityTooLargeError));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}

TEST(FuncResult_TestCase, RaiseFileDoesNotExistError)
{
	using http::error_type;
	auto result = http::raise<error_type::FileDoesNotExistError, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::FileDoesNotExistError));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}

TEST(FuncResult_TestCase, RaisePermissionDenied)
{
	using http::error_type;
	auto result = http::raise<error_type::PermissionDenied, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::PermissionDenied));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}

TEST(FuncResult_TestCase, RaiseSuspiciousOperation)
{
	using http::error_type;
	auto result = http::raise<error_type::SuspiciousOperation, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::SuspiciousOperation));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}

TEST(FuncResult_TestCase, RaiseDisallowedHost)
{
	using http::error_type;
	auto result = http::raise<error_type::DisallowedHost, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::DisallowedHost));
	ASSERT_TRUE(result.catch_(error_type::SuspiciousOperation));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}

TEST(FuncResult_TestCase, RaiseDisallowedRedirect)
{
	using http::error_type;
	auto result = http::raise<error_type::DisallowedRedirect, int>("an error");
	ASSERT_TRUE(result.err);
	ASSERT_TRUE(result.catch_(error_type::DisallowedRedirect));
	ASSERT_TRUE(result.catch_(error_type::SuspiciousOperation));
	ASSERT_TRUE(result.catch_(error_type::HttpError));
}
