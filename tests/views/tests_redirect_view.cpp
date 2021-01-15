/**
 * views/tests_redirect_view.cpp
 *
 * Copyright (c) 2019 Yuriy Lisovskiy
 */

#include <map>

#include <gtest/gtest.h>

#include <xalwart.core/logger.h>
#include <xalwart.core/collections/dict.h>
#include <xalwart.core/collections/multi_dict.h>

#include "../../src/views/redirect_view.h"
#include "../../src/conf/settings.h"

using namespace xw;


http::HttpRequest make_request(const conf::Settings* settings, const std::string& method)
{
	auto empty_parameters = http::HttpRequest::Parameters<std::string, xw::string>(
		collections::Dict<std::string, xw::string>(),
		collections::MultiValueDict<std::string, xw::string>()
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


struct RedirectViewTestSettings : public conf::Settings
{
	RedirectViewTestSettings() : conf::Settings("./")
	{
	}

	void register_logger() final
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


class RedirectViewWithDefaultParamsTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectViewTestSettings();
		this->settings->init();
		this->view = new views::RedirectView(this->settings, "/hello");
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewWithDefaultParamsTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseRedirect*>(response.value.get()));
}

TEST_F(RedirectViewWithDefaultParamsTestCase, TraceTest)
{
	auto request = make_request(this->settings, "trace");
	auto response = this->view->trace(&request, nullptr);

	ASSERT_EQ(response.value, nullptr);
}

TEST_F(RedirectViewWithDefaultParamsTestCase, AllowedMethodsTest)
{
	auto expected = std::vector<std::string>{"GET", "POST", "PUT", "PATCH", "DELETE", "HEAD", "OPTIONS"};
	auto actual = this->view->allowed_methods();

	ASSERT_EQ(expected.size(), actual.size());

	for (size_t i = 0; i < expected.size(); i++)
	{
		ASSERT_EQ(expected[i], actual[i]);
	}
}

TEST_F(RedirectViewWithDefaultParamsTestCase, SetupAndDispatchAllowedTest)
{
	auto request = make_request(this->settings, "get");

	ASSERT_THROW(this->view->dispatch(nullptr), core::NullPointerException);

	this->view->setup(&request);
	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 302);
}


TEST_F(RedirectViewWithDefaultParamsTestCase, DispatchTraceNotAllowedTest)
{
	auto request = make_request(this->settings, "trace");
	this->view->setup(&request);

	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response.value->status(), 405);
}

TEST_F(RedirectViewWithDefaultParamsTestCase, GetRedirectUrlTest)
{
	auto request = make_request(this->settings, "get");
	this->view->setup(&request);
	ASSERT_EQ(this->view->get_redirect_url(), "/hello");
}


class RedirectViewPermanentAndQueryStringTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectViewTestSettings();
		this->settings->init();
		this->view = new views::RedirectView(
			this->settings, "/hello/world", true, true
		);
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewPermanentAndQueryStringTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponsePermanentRedirect*>(response.value.get()));
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, GetRedirectUrlTest)
{
	auto empty_parameters = http::HttpRequest::Parameters<std::string, xw::string>(
		collections::Dict<std::string, xw::string>(),
		collections::MultiValueDict<std::string, xw::string>()
	);
	auto empty_map = collections::Dict<std::string, std::string>();
	auto* request = new http::HttpRequest(
		this->settings,
		"get",
		"/hello",
		1, 1,
		"param1=100&q=find_user",
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
	this->view->setup(request);

	ASSERT_EQ(this->view->get_redirect_url(), "/hello/world?param1=100&q=find_user");

	delete request;
}


class RedirectViewEmptyUrlTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings = nullptr;

	void SetUp() override
	{
		this->settings = new RedirectViewTestSettings();
		this->settings->init();
		this->view = new views::RedirectView(this->settings, "");
	}

	void TearDown() override
	{
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewEmptyUrlTestCase, GetTest)
{
	auto request = make_request(this->settings, "get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, PostTest)
{
	auto request = make_request(this->settings, "post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, PutTest)
{
	auto request = make_request(this->settings, "put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, PatchTest)
{
	auto request = make_request(this->settings, "patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, DeleteTest)
{
	auto request = make_request(this->settings, "delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, HeadTest)
{
	auto request = make_request(this->settings, "head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, OptionsTest)
{
	auto request = make_request(this->settings, "options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_TRUE(dynamic_cast<http::HttpResponseGone*>(response.value.get()));
}

TEST_F(RedirectViewEmptyUrlTestCase, GetRedirectUrlTest)
{
	auto request = make_request(this->settings, "get");
	this->view->setup(&request);

	ASSERT_EQ(this->view->get_redirect_url(), "");
}
