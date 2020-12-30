/**
 * views/tests_args.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include "../../src/views/args.h"

using namespace xw;


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
