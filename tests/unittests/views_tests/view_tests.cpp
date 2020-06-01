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

#include "../../../src/views/view.h"
#include "../../../src/conf/settings.h"
#include "../../../src/core/strings.h"
#include "../../../src/http/request.h"
#include "../../../src/collections/dict.h"
#include "../../../src/collections/multi_dict.h"

using namespace xw;


struct ViewTestSettings : public conf::Settings
{
	ViewTestSettings() : conf::Settings()
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

class ViewTestCase : public ::testing::Test
{
public:
	static http::HttpRequest make_request(const std::string& method)
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

protected:
	views::View* view = nullptr;
	ViewTestSettings* settings;

	void SetUp() override
	{
		this->settings = new ViewTestSettings();
		this->settings->init();
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
	auto request = ViewTestCase::make_request("get");
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(response, nullptr);
}

TEST_F(ViewTestCase, PostTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("post");
	ASSERT_EQ(this->view->post(&request, nullptr), nullptr);
}

TEST_F(ViewTestCase, PutTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("put");
	ASSERT_EQ(this->view->put(&request, nullptr), nullptr);
}

TEST_F(ViewTestCase, PatchTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("patch");
	ASSERT_EQ(this->view->patch(&request, nullptr), nullptr);
}

TEST_F(ViewTestCase, DeleteTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("delete");
	ASSERT_EQ(this->view->delete_(&request, nullptr), nullptr);
}

TEST_F(ViewTestCase, HeadTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("head");
	ASSERT_EQ(this->view->head(&request, nullptr), nullptr);
}

TEST_F(ViewTestCase, OptionsTest)
{
	auto expected_response = http::HttpResponse("");
	auto vec = std::vector<std::string>{"get", "post", "head", "options"};
	expected_response.set_header(
		"Allow",
		core::str::join(vec.cbegin(), vec.cend(), ", ")
	);
	expected_response.set_header("Content-Length", "0");

	auto request = ViewTestCase::make_request("options");
	auto actual_response = this->view->options(&request, nullptr);

	ASSERT_EQ(actual_response->content_type(), expected_response.content_type());
	ASSERT_EQ(actual_response->status(), expected_response.status());
	ASSERT_EQ(actual_response->charset(), expected_response.charset());
}

TEST_F(ViewTestCase, TraceTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("trace");
	ASSERT_EQ(this->view->trace(&request, nullptr), nullptr);
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
	auto request = ViewTestCase::make_request("options");

	ASSERT_THROW(this->view->dispatch(nullptr), core::NullPointerException);

	this->view->setup(&request);
	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 200);
}

TEST_F(ViewTestCase, DispatchNotAllowedTest)
{
	auto request = ViewTestCase::make_request("get");
	this->view->setup(&request);

	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 405);
}
