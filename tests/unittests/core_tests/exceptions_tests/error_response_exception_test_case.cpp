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

#include "../../../../src/core/exceptions.h"

using namespace xw;


class ErrorResponseExceptionTestCase : public ::testing::Test
{
public:
	const char* WhatErrorMessage = "Test error message from ErrorResponseExceptionTestCase";
	const char* FullErrorMessage = "ErrorResponseException: Test error message from ErrorResponseExceptionTestCase";
	const char* ErrorFunction = "testFunc";
	const char* ErrorFile = "test_file.cpp";
	const size_t ErrorLine = 1;
	const short int StatusCode = 400;

	core::ErrorResponseException ConstCharConstructorError;
	core::ErrorResponseException StringConstructorError;

	explicit ErrorResponseExceptionTestCase()
		: ConstCharConstructorError(400, "Test error message from ErrorResponseExceptionTestCase", this->ErrorLine, this->ErrorFunction, this->ErrorFile),
		  StringConstructorError(400, std::string("Test error message from ErrorResponseExceptionTestCase"), this->ErrorLine, this->ErrorFunction, this->ErrorFile)
	{
	}
};

TEST_F(ErrorResponseExceptionTestCase, TestWhat)
{
	ASSERT_STREQ(this->ConstCharConstructorError.what(), this->WhatErrorMessage);
	ASSERT_STREQ(this->StringConstructorError.what(), this->WhatErrorMessage);
}

TEST_F(ErrorResponseExceptionTestCase, TestLine)
{
	ASSERT_EQ(this->ConstCharConstructorError.line(), this->ErrorLine);
	ASSERT_EQ(this->StringConstructorError.line(), this->ErrorLine);
}

TEST_F(ErrorResponseExceptionTestCase, TestFunction)
{
	ASSERT_EQ(this->ConstCharConstructorError.function(), this->ErrorFunction);
	ASSERT_EQ(this->StringConstructorError.function(), this->ErrorFunction);
}

TEST_F(ErrorResponseExceptionTestCase, TestFile)
{
	ASSERT_EQ(this->ConstCharConstructorError.file(), this->ErrorFile);
	ASSERT_EQ(this->StringConstructorError.file(), this->ErrorFile);
}

TEST_F(ErrorResponseExceptionTestCase, TestGetMessage)
{
	ASSERT_EQ(this->ConstCharConstructorError.get_message(), this->FullErrorMessage);
	ASSERT_EQ(this->StringConstructorError.get_message(), this->FullErrorMessage);
}

TEST_F(ErrorResponseExceptionTestCase, TestStatusCode)
{
	ASSERT_EQ(this->ConstCharConstructorError.status_code(), this->StatusCode);
	ASSERT_EQ(this->StringConstructorError.status_code(), this->StatusCode);
}
