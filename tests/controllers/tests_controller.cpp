/**
 * controllers/tests_controller.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include <xalwart.base/string_utils.h>
#include <xalwart.base/collections/dictionary.h>
#include <xalwart.base/collections/multi_dictionary.h>

#include "../../src/controllers/controller.h"
#include "../../src/conf/settings.h"

using namespace xw;


struct ControllerTestSettings : public conf::Settings
{
	ControllerTestSettings() : conf::Settings("./")
	{
		auto lc = log::Config();
		lc.enable_info = false;
		lc.enable_debug = false;
		lc.enable_warning = false;
		lc.enable_error = false;
		lc.enable_fatal = false;
		lc.enable_print = false;
		this->LOGGER = std::make_shared<log::Logger>(lc);
	}
};

class ControllerTestCase : public ::testing::Test
{
public:
	static http::HttpRequest make_request(const conf::Settings* settings, const std::string& method)
	{
		auto empty_parameters = collections::MultiDictionary<std::string, std::string>();
		auto empty_map = collections::Dictionary<std::string, std::string>();
		return http::HttpRequest(
			settings,
			method,
			"/hello",
			1, 1,
			"",
			true,
			"",
			empty_map,
			empty_parameters,
			empty_parameters,
			collections::MultiDictionary<std::string, files::UploadedFile>(),
			{}
		);
	}

protected:
	ctrl::Controller* controller = nullptr;
	ControllerTestSettings* settings;

	void SetUp() override
	{
		this->settings = new ControllerTestSettings();
		this->controller = new ctrl::Controller(this->settings);
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->controller;
	}
};

TEST_F(ControllerTestCase, GetTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "get");
	auto response = this->controller->get(nullptr);

	ASSERT_EQ(response.value, nullptr);
}

TEST_F(ControllerTestCase, PostTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "post");
	ASSERT_EQ(this->controller->post(nullptr).value, nullptr);
}

TEST_F(ControllerTestCase, PutTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "put");
	ASSERT_EQ(this->controller->put(nullptr).value, nullptr);
}

TEST_F(ControllerTestCase, PatchTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "patch");
	ASSERT_EQ(this->controller->patch(nullptr).value, nullptr);
}

TEST_F(ControllerTestCase, DeleteTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "delete");
	ASSERT_EQ(this->controller->delete_(nullptr).value, nullptr);
}

TEST_F(ControllerTestCase, HeadTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "head");
	ASSERT_EQ(this->controller->head(nullptr).value, nullptr);
}

TEST_F(ControllerTestCase, OptionsTest)
{
	auto expected_response = http::HttpResponse();
	auto vec = std::vector<std::string>{"get", "post", "head", "options"};
	expected_response.set_header(
		"Allow",
		str::join(", ", vec.cbegin(), vec.cend())
	);
	expected_response.set_header("Content-Length", "0");

	auto request = ControllerTestCase::make_request(this->settings, "options");
	auto actual_response = this->controller->options(nullptr);

	ASSERT_EQ(actual_response.value->content_type(), expected_response.content_type());
	ASSERT_EQ(actual_response.value->status(), expected_response.status());
	ASSERT_EQ(actual_response.value->charset(), expected_response.charset());
}

TEST_F(ControllerTestCase, TraceTestReturnsNullptr)
{
	auto request = ControllerTestCase::make_request(this->settings, "trace");
	ASSERT_EQ(this->controller->trace(nullptr).value, nullptr);
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
	auto request = ControllerTestCase::make_request(this->settings, "options");

	ASSERT_THROW(this->controller->dispatch(nullptr), NullPointerException);

	this->controller->setup(&request);
	auto response = this->controller->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 200);
}

TEST_F(ControllerTestCase, DispatchNotAllowedTest)
{
	auto request = ControllerTestCase::make_request(this->settings, "get");
	this->controller->setup(&request);

	auto response = this->controller->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 405);
}
