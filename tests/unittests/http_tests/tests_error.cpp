/**
 * http/tests_error.cpp
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/http/error.h"


TEST(FuncResult_TestCase, ConstructError)
{
	using xw::http::error_type;
	auto err = xw::http::error(error_type::HttpError, "an error", 1, "main", "main.cpp");
	ASSERT_TRUE(err);
	ASSERT_EQ(err.type, error_type::HttpError);
	ASSERT_EQ(err.msg, "an error");
	ASSERT_EQ(err.line, 1);
	ASSERT_EQ(err.func, "main");
	ASSERT_EQ(err.file, "main.cpp");
}
