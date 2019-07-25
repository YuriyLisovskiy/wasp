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

#ifndef WASP_UNIT_TESTS_CORE_EXCEPTIONS_WASP_ERROR_TESTS_H
#define WASP_UNIT_TESTS_CORE_EXCEPTIONS_WASP_ERROR_TESTS_H

#include <gtest/gtest.h>

#include "../../globals.h"
#include "../../../../src/core/exceptions.h"


__UNIT_TESTS_BEGIN__

class BaseExceptionTestCase : public ::testing::Test
{
public:
	const char* FullErrorMessage = "BaseException: Test error message from BaseExceptionTestCase";
	const size_t ErrorLine = 1;
	const char* ErrorFunction = "testFunc";
	const char* ErrorFile = "test_file.cpp";

	wasp::BaseException Error;

	explicit BaseExceptionTestCase()
		: Error("Test error message from BaseExceptionTestCase", this->ErrorLine, this->ErrorFunction, this->ErrorFile)
	{
	}
};

TEST_F(BaseExceptionTestCase, TestWhat)
{
	ASSERT_STREQ(this->Error.what(), this->FullErrorMessage);
}

TEST_F(BaseExceptionTestCase, TestLine)
{
	ASSERT_EQ(this->Error.line(), this->ErrorLine);
}

TEST_F(BaseExceptionTestCase, TestFunction)
{
	ASSERT_EQ(this->Error.function(), this->ErrorFunction);
}

TEST_F(BaseExceptionTestCase, TestFile)
{
	ASSERT_EQ(this->Error.file(), this->ErrorFile);
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_CORE_EXCEPTIONS_WASP_ERROR_TESTS_H
