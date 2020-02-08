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

#ifndef WASP_UNIT_TESTS_VIEWS_TESTS_REDIRECT_VIEW_TESTS_H
#define WASP_UNIT_TESTS_VIEWS_TESTS_REDIRECT_VIEW_TESTS_H

#include <map>

#include <gtest/gtest.h>

#include "../_def_.h"
#include "../../../src/views/redirect_view.h"
#include "../../../src/http/request.h"
#include "../../../src/conf/settings.h"
#include "../../../src/core/logger.h"
#include "../../../src/collections/dict.h"
#include "../../../src/collections/multi_dict.h"


__UNIT_TESTS_BEGIN__

http::HttpRequest make_request(const std::string& method)
{
	auto empty_parameters = http::HttpRequest::Parameters<std::string, std::string>(
		collections::Dict<std::string, std::string>(),
		collections::MultiValueDict<std::string, std::string>()
	);
	auto empty_map = std::map<std::string, std::string>();
	return http::HttpRequest(
		method,
		"/hello",
		1, 1,
		"",
		true,
		"",
		empty_map,
		empty_parameters,
		empty_parameters,
		http::HttpRequest::Parameters<std::string, core::UploadedFile>(
			collections::Dict<std::string, core::UploadedFile>(),
			collections::MultiValueDict<std::string, core::UploadedFile>()
		)
	);
}


struct RedirectViewTestSettings : public conf::Settings
{
	RedirectViewTestSettings() : conf::Settings()
	{
	}

	void init() final
	{
		this->LOGGER = core::Logger::get_instance({
			false,
			false,
			false,
			false,
			false,
			false
		});
	}
};


class RedirectViewWithDefaultParamsTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings;

	void SetUp() override
	{
		this->settings = new RedirectViewTestSettings();
		this->settings->init();
		this->view = new views::RedirectView(this->settings, "/hello");
	}

	void TearDown() override
	{
		core::Logger::reset_instance();
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewWithDefaultParamsTestCase, GetTest)
{
	auto request = make_request("get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PostTest)
{
	auto request = make_request("post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PutTest)
{
	auto request = make_request("put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, PatchTest)
{
	auto request = make_request("patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, DeleteTest)
{
	auto request = make_request("delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, HeadTest)
{
	auto request = make_request("head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, OptionsTest)
{
	auto request = make_request("options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, TraceTest)
{
	auto request = make_request("trace");
	auto response = this->view->trace(&request, nullptr);

	ASSERT_EQ(response, nullptr);

	delete response;
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
	auto request = make_request("get");

	ASSERT_THROW(this->view->dispatch(nullptr), core::NullPointerException);

	this->view->setup(&request);
	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 302);

	delete response;
}


TEST_F(RedirectViewWithDefaultParamsTestCase, DispatchTraceNotAllowedTest)
{
	auto request = make_request("trace");
	this->view->setup(&request);

	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 405);

	delete response;
}

TEST_F(RedirectViewWithDefaultParamsTestCase, GetRedirectUrlTest)
{
	auto request = make_request("get");
	this->view->setup(&request);
	ASSERT_EQ(this->view->get_redirect_url(), "/hello");
}


class RedirectViewPermanentAndQueryStringTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings;

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
		core::Logger::reset_instance();
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewPermanentAndQueryStringTestCase, GetTest)
{
	auto request = make_request("get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PostTest)
{
	auto request = make_request("post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PutTest)
{
	auto request = make_request("put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, PatchTest)
{
	auto request = make_request("patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, DeleteTest)
{
	auto request = make_request("delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, HeadTest)
{
	auto request = make_request("head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, OptionsTest)
{
	auto request = make_request("options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponsePermanentRedirect), typeid(*response));

	delete response;
}

TEST_F(RedirectViewPermanentAndQueryStringTestCase, GetRedirectUrlTest)
{
	auto empty_parameters = http::HttpRequest::Parameters<std::string, std::string>(
		collections::Dict<std::string, std::string>(),
		collections::MultiValueDict<std::string, std::string>()
	);
	auto empty_map = std::map<std::string, std::string>();
	auto* request = new http::HttpRequest(
		"get",
		"/hello",
		1, 1,
		"param1=100&q=find_user",
		true,
		"",
		empty_map,
		empty_parameters,
		empty_parameters,
		http::HttpRequest::Parameters<std::string, core::UploadedFile>(
			collections::Dict<std::string, core::UploadedFile>(),
			collections::MultiValueDict<std::string, core::UploadedFile>()
		)
	);
	this->view->setup(request);

	ASSERT_EQ(this->view->get_redirect_url(), "/hello/world?param1=100&q=find_user");

	delete request;
}


class RedirectViewEmptyUrlTestCase : public ::testing::Test
{
protected:
	views::RedirectView* view = nullptr;
	RedirectViewTestSettings* settings;

	void SetUp() override
	{
		this->settings = new RedirectViewTestSettings();
		this->settings->init();
		this->view = new views::RedirectView(this->settings, "");
	}

	void TearDown() override
	{
		core::Logger::reset_instance();
		delete this->settings;
		delete this->view;
	}
};

TEST_F(RedirectViewEmptyUrlTestCase, GetTest)
{
	auto request = make_request("get");
	this->view->setup(&request);
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, PostTest)
{
	auto request = make_request("post");
	this->view->setup(&request);
	auto response = this->view->post(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, PutTest)
{
	auto request = make_request("put");
	this->view->setup(&request);
	auto response = this->view->put(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, PatchTest)
{
	auto request = make_request("patch");
	this->view->setup(&request);
	auto response = this->view->patch(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, DeleteTest)
{
	auto request = make_request("delete");
	this->view->setup(&request);
	auto response = this->view->delete_(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, HeadTest)
{
	auto request = make_request("head");
	this->view->setup(&request);
	auto response = this->view->head(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, OptionsTest)
{
	auto request = make_request("options");
	this->view->setup(&request);
	auto response = this->view->options(&request, nullptr);

	ASSERT_EQ(typeid(http::HttpResponseGone), typeid(*response));

	delete response;
}

TEST_F(RedirectViewEmptyUrlTestCase, GetRedirectUrlTest)
{
	auto request = make_request("get");
	this->view->setup(&request);

	ASSERT_EQ(this->view->get_redirect_url(), "");
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_VIEWS_TESTS_REDIRECT_VIEW_TESTS_H
