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

#include <map>

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/views/args.h"


__UNIT_TESTS_BEGIN__

class ArgsTestCase : public ::testing::Test
{
protected:
	views::Args* args = nullptr;

	void SetUp() override
	{
		this->args = new views::Args(std::map<std::string, std::string>{
			{"user_id", "12345"},
			{"account_name", "some_name"}
		});
	}

	void TearDown() override
	{
		delete this->args;
	}
};

TEST_F(ArgsTestCase, ContainsTest)
{
	ASSERT_FALSE(this->args->contains("user_account_id"));
	ASSERT_TRUE(this->args->contains("account_name"));
}

TEST_F(ArgsTestCase, GetIntTest)
{
	ASSERT_EQ(this->args->get_int("user_id"), 12345);
}

TEST_F(ArgsTestCase, GetNonIntTest)
{
	ASSERT_EQ(this->args->get_int("account_name", -1), -1);
}

TEST_F(ArgsTestCase, GetIntTestReturnsDefault)
{
	ASSERT_EQ(this->args->get_int("user", -1), -1);
}

TEST_F(ArgsTestCase, GetStringTest)
{
	ASSERT_EQ(this->args->get_str("account_name"), "some_name");
	ASSERT_EQ(this->args->get_str("user_id"), "12345");
}

TEST_F(ArgsTestCase, GetStringTestReturnsDefault)
{
	ASSERT_EQ(this->args->get_str("account", "default value"), "default value");
}

__UNIT_TESTS_END__
