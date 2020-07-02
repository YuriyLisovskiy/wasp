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


class BaseExceptionTestCase : public ::testing::Test
{
public:
	const char* WhatErrorMessage = "Test error message from BaseExceptionTestCase";
	const char* FullErrorMessage = "BaseException: Test error message from BaseExceptionTestCase";
	const char* ErrorFunction = "testFunc";
	const char* ErrorFile = "test_file.cpp";
	const size_t ErrorLine = 1;

	core::BaseException ConstCharConstructorError;

	explicit BaseExceptionTestCase()
		: ConstCharConstructorError("Test error message from BaseExceptionTestCase", this->ErrorLine, this->ErrorFunction, this->ErrorFile)
	{
	}
};

TEST_F(BaseExceptionTestCase, TestWhat)
{
	ASSERT_STREQ(this->ConstCharConstructorError.what(), this->WhatErrorMessage);
}

TEST_F(BaseExceptionTestCase, TestLine)
{
	ASSERT_EQ(this->ConstCharConstructorError.line(), this->ErrorLine);
}

TEST_F(BaseExceptionTestCase, TestFunction)
{
	ASSERT_EQ(this->ConstCharConstructorError.function(), this->ErrorFunction);
}

TEST_F(BaseExceptionTestCase, TestFile)
{
	ASSERT_EQ(this->ConstCharConstructorError.file(), this->ErrorFile);
}

TEST_F(BaseExceptionTestCase, TestGetMessage)
{
	ASSERT_EQ(this->ConstCharConstructorError.get_message(), this->FullErrorMessage);
}
