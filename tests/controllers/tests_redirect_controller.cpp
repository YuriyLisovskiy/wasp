/**
 * controllers/tests_redirect_controller.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include <xalwart.base/logger.h>

#include "../../src/conf/settings.h"
#include "../../src/controllers/redirect.h"

using namespace xw;


http::Request make_request(const conf::Settings* settings, const std::string& method)
{
	auto context = net::RequestContext{
		.protocol_version = {
			.major = 1,
			.minor = 1
		},
		.path = "/hello",
		.method = method
	};
	return http::Request(context, 99999, 99, 9999, 99, 9999, {});
}

struct RedirectControllerTestSettings : public conf::Settings
{
	RedirectControllerTestSettings() : conf::Settings("./")
	{
		auto lc = log::Config();
		lc.disable_all_levels();
		this->LOGGER = std::make_shared<log::Logger>(lc);
	}
};


class RedirectControllerWithDefaultParamsTestCase : public ::testing::Test
{
protected:
	ctrl::RedirectController* controller = nullptr;
	RedirectControllerTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectControllerTestSettings();
		this->controller = new ctrl::RedirectController(this->settings, "/hello");
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->controller;
	}
};

TEST_F(RedirectControllerWithDefaultParamsTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	auto response = this->controller->get(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	auto response = this->controller->post(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	auto response = this->controller->put(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	auto response = this->controller->patch(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	auto response = this->controller->delete_(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	auto response = this->controller->head(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	auto response = this->controller->options(&request);

	ASSERT_TRUE(dynamic_cast<http::Redirect*>(response.get()));
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, TraceTest)
{
	auto request = make_request(this->settings, "trace");
	auto response = this->controller->trace(&request);

	ASSERT_EQ(response, nullptr);
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, AllowedMethodsTest)
{
	auto expected = std::vector<std::string>{"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
	auto actual = this->controller->allowed_methods();

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, SetupAndDispatchAllowedTest)
{
	ASSERT_THROW(auto _ = this->controller->dispatch(nullptr), NullPointerException);

	auto request = make_request(this->settings, "get");
	auto response = this->controller->dispatch(&request);

	ASSERT_EQ(response->get_status(), 302);
}


TEST_F(RedirectControllerWithDefaultParamsTestCase, DispatchTraceNotAllowedTest)
{
	auto request = make_request(this->settings, "trace");

	auto response = this->controller->dispatch(&request);

	ASSERT_EQ(response->get_status(), 405);
}

TEST_F(RedirectControllerWithDefaultParamsTestCase, GetRedirectUrlTest)
{
	auto request = make_request(this->settings, "get");
	ASSERT_EQ(this->controller->get_redirect_url(&request), "/hello");
}


class RedirectControllerPermanentAndQueryStringTestCase : public ::testing::Test
{
protected:
	ctrl::RedirectController* controller = nullptr;
	RedirectControllerTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectControllerTestSettings();
		this->controller = new ctrl::RedirectController(
			this->settings, "/hello/world", true, true
		);
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->controller;
	}
};

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	auto response = this->controller->get(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	auto response = this->controller->post(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	auto response = this->controller->put(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	auto response = this->controller->patch(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	auto response = this->controller->delete_(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	auto response = this->controller->head(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	auto response = this->controller->options(&request);

	ASSERT_TRUE(dynamic_cast<http::PermanentRedirect*>(response.get()));
}

TEST_F(RedirectControllerPermanentAndQueryStringTestCase, GetRedirectUrlTest)
{
	auto context = net::RequestContext{
		.protocol_version = {
			.major = 1,
			.minor = 1
		},
		.path = "/hello/world",
		.query = "param1=100&q=find_user&qqq=собака",
		.method = "get"
	};
	auto request = http::Request(context, 99999, 99, 9999, 99, 9999, {});
	ASSERT_EQ(
		this->controller->get_redirect_url(&request), "/hello/world?param1=100&q=find_user&qqq=собака"
	);
}

class RedirectControllerEmptyUrlTestCase : public ::testing::Test
{
protected:
	ctrl::RedirectController* controller = nullptr;
	RedirectControllerTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectControllerTestSettings();
		this->controller = new ctrl::RedirectController(this->settings, "");
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->controller;
	}
};

TEST_F(RedirectControllerEmptyUrlTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	auto response = this->controller->get(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	auto response = this->controller->post(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	auto response = this->controller->put(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	auto response = this->controller->patch(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	auto response = this->controller->delete_(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	auto response = this->controller->head(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	auto response = this->controller->options(&request);

	ASSERT_TRUE(dynamic_cast<http::Gone*>(response.get()));
}

TEST_F(RedirectControllerEmptyUrlTestCase, GetRedirectUrlTest)
{
	auto request = make_request(this->settings, "get");

	ASSERT_EQ(this->controller->get_redirect_url(&request), "");
}
