/**
 * core/tests_kwargs.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include "../../src/core/kwargs.h"

using namespace xw;


class KwargsTestCase : public ::testing::Test
{
protected:
	core::Kwargs* kwargs = nullptr;

	void SetUp() override
	{
		this->kwargs = new core::Kwargs(std::map<std::string, std::string>{
			{"user_id", "12345"},
			{"account_name", "some_name"}
		});
	}

	void TearDown() override
	{
		delete this->kwargs;
	}
};

TEST_F(KwargsTestCase, ContainsTest)
{
	ASSERT_FALSE(this->kwargs->contains("user_account_id"));
	ASSERT_TRUE(this->kwargs->contains("account_name"));
}

TEST_F(KwargsTestCase, GetIntTest)
{
	ASSERT_EQ(this->kwargs->get<int>("user_id"), 12345);
}

TEST_F(KwargsTestCase, GetNonIntTest)
{
	ASSERT_EQ(this->kwargs->get<int>("account_name", -1), -1);
}

TEST_F(KwargsTestCase, GetIntTestReturnsDefault)
{
	ASSERT_EQ(this->kwargs->get<int>("user", -1), -1);
}

TEST_F(KwargsTestCase, GetStringTest)
{
	ASSERT_EQ(this->kwargs->get<std::string>("account_name"), "some_name");
	ASSERT_EQ(this->kwargs->get<std::string>("user_id"), "12345");
}

TEST_F(KwargsTestCase, GetStringTestReturnsDefault)
{
	ASSERT_EQ(this->kwargs->get<std::string>("account", "default value"), "default value");
}
