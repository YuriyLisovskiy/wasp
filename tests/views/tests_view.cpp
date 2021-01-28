/**
 * views/tests_view.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include <xalwart.core/string_utils.h>
#include <xalwart.core/collections/dict.h>
#include <xalwart.core/collections/multi_dict.h>

#include "../../src/views/view.h"
#include "../../src/conf/settings.h"

using namespace xw;


struct ViewTestSettings : public conf::Settings
{
	ViewTestSettings() : conf::Settings("./")
	{
		auto lc = core::LoggerConfig();
		lc.enable_info = false;
		lc.enable_debug = false;
		lc.enable_warning = false;
		lc.enable_error = false;
		lc.enable_fatal = false;
		lc.enable_print = false;
		this->LOGGER = core::Logger::get_instance(lc);
	}
};

class ViewTestCase : public ::testing::Test
{
public:
	static http::HttpRequest make_request(const conf::Settings* settings, const std::string& method)
	{
		auto empty_parameters = http::HttpRequest::Parameters<std::string, std::string>(
			collections::Dict<std::string, std::string>(),
			collections::MultiValueDict<std::string, std::string>()
		);
		auto empty_map = collections::Dict<std::string, std::string>();
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
			http::HttpRequest::Parameters<std::string, files::UploadedFile>(
				collections::Dict<std::string, files::UploadedFile>(),
				collections::MultiValueDict<std::string, files::UploadedFile>()
			),
			{}
		);
	}

protected:
	views::View* view = nullptr;
	ViewTestSettings* settings;

	void SetUp() override
	{
		this->settings = new ViewTestSettings();
		this->view = new views::View(this->settings);
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->view;
	}
};

TEST_F(ViewTestCase, GetTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "get");
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(response.value, nullptr);
}

TEST_F(ViewTestCase, PostTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "post");
	ASSERT_EQ(this->view->post(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, PutTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "put");
	ASSERT_EQ(this->view->put(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, PatchTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "patch");
	ASSERT_EQ(this->view->patch(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, DeleteTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "delete");
	ASSERT_EQ(this->view->delete_(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, HeadTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "head");
	ASSERT_EQ(this->view->head(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, OptionsTest)
{
	auto expected_response = http::HttpResponse();
	auto vec = std::vector<std::string>{"get", "post", "head", "options"};
	expected_response.set_header(
		"Allow",
		str::join(vec.cbegin(), vec.cend(), ", ")
	);
	expected_response.set_header("Content-Length", "0");

	auto request = ViewTestCase::make_request(this->settings, "options");
	auto actual_response = this->view->options(&request, nullptr);

	ASSERT_EQ(actual_response.value->content_type(), expected_response.content_type());
	ASSERT_EQ(actual_response.value->status(), expected_response.status());
	ASSERT_EQ(actual_response.value->charset(), expected_response.charset());
}

TEST_F(ViewTestCase, TraceTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request(this->settings, "trace");
	ASSERT_EQ(this->view->trace(&request, nullptr).value, nullptr);
}

TEST_F(ViewTestCase, AllowedMethodsTest)
{
	auto expected = std::vector<std::string>{"OPTIONS"};
	auto actual = this->view->allowed_methods();

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST_F(ViewTestCase, SetupAndDispatchAllowedTest)
{
	auto request = ViewTestCase::make_request(this->settings, "options");

	ASSERT_THROW(this->view->dispatch(nullptr), core::NullPointerException);

	this->view->setup(&request);
	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 200);
}

TEST_F(ViewTestCase, DispatchNotAllowedTest)
{
	auto request = ViewTestCase::make_request(this->settings, "get");
	this->view->setup(&request);

	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 405);
}
