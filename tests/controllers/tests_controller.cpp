/**
 * controllers/tests_controller.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include <xalwart.base/string_utils.h>
#include <xalwart.base/logger.h>

#include "../../src/controllers/controller.h"

using namespace xw;


class ControllerTestCase : public ::testing::Test
{
public:
	static http::Request make_request(const std::string& method)
	{
		auto context = net::RequestContext{
			.method = method
		};
		return http::Request(context, 99999, 99, 9999, 99, 9999, {});
	}

protected:
	ctrl::Controller<>* controller = nullptr;
	std::shared_ptr<ILogger> logger = nullptr;

	void SetUp() override
	{
		auto lc = log::Config();
		lc.disable_all_levels();
		this->logger = std::make_shared<log::Logger>(lc);
		this->controller = new ctrl::Controller<>(this->logger.get());
	}

	void TearDown() override
	{
		delete this->controller;
	}
};

TEST_F(ControllerTestCase, GetTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("get");
	auto response = this->controller->get(&request);

	ASSERT_EQ(response, nullptr);
}

TEST_F(ControllerTestCase, PostTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("post");
	ASSERT_EQ(this->controller->post(&request), nullptr);
}

TEST_F(ControllerTestCase, PutTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("put");
	ASSERT_EQ(this->controller->put(&request), nullptr);
}

TEST_F(ControllerTestCase, PatchTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("patch");
	ASSERT_EQ(this->controller->patch(&request), nullptr);
}

TEST_F(ControllerTestCase, DeleteTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("delete");
	ASSERT_EQ(this->controller->delete_(&request), nullptr);
}

TEST_F(ControllerTestCase, HeadTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("head");
	ASSERT_EQ(this->controller->head(&request), nullptr);
}

TEST_F(ControllerTestCase, OptionsTest)
{
	auto expected_response = http::Response();
	auto vec = std::vector<std::string>{"get", "post", "head", "options"};
	expected_response.set_header(
		"Allow", str::join(", ", vec.cbegin(), vec.cend())
	);
	expected_response.set_header("Content-Length", "0");

	auto request = ControllerTestCase::make_request("options");
	auto actual_response = this->controller->options(&request);

	ASSERT_EQ(actual_response->content_type(), expected_response.content_type());
	ASSERT_EQ(actual_response->get_status(), expected_response.get_status());
	ASSERT_EQ(actual_response->get_charset(), expected_response.get_charset());
}

TEST_F(ControllerTestCase, TraceTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request("trace");
	ASSERT_EQ(this->controller->trace(&request), nullptr);
}

TEST_F(ControllerTestCase, AllowedMethodsTest)
{
	auto expected = std::vector<std::string>{"OPTIONS"};
	auto actual = this->controller->allowed_methods();

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST_F(ControllerTestCase, SetupAndDispatchAllowedTest)
{
	ASSERT_THROW(auto _ = this->controller->dispatch(nullptr), NullPointerException);

	auto request = ControllerTestCase::make_request("options");
	auto response = this->controller->dispatch(&request);

	ASSERT_EQ(response->get_status(), 200);
}

TEST_F(ControllerTestCase, DispatchNotAllowedTest)
{
	auto request = ControllerTestCase::make_request("get");

	auto response = this->controller->dispatch(&request);

	ASSERT_EQ(response->get_status(), 405);
}
