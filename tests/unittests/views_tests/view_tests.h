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

#ifndef WASP_UNIT_TESTS_VIEWS_TESTS_VIEW_TESTS_H
#define WASP_UNIT_TESTS_VIEWS_TESTS_VIEW_TESTS_H

#include <map>

#include <gtest/gtest.h>

#include "../globals.h"
#include "../../../src/views/view.h"
#include "../../../src/http/request.h"
#include "../../../src/collections/dict.h"
#include "../../../src/collections/multi_dict.h"


__UNIT_TESTS_BEGIN__

class ViewTestCase : public ::testing::Test
{
protected:
	View* view = nullptr;

	static HttpRequest make_request(const std::string& method)
	{
		auto empty_parameters = HttpRequest::Parameters<std::string, std::string>(
			Dict<std::string, std::string>(),
			MultiValueDict<std::string, std::string>()
		);
		auto empty_map = std::map<std::string, std::string>();
		return HttpRequest(
			method,
			"/hello",
			1, 1,
			"",
			true,
			"",
			empty_map,
			empty_parameters,
			empty_parameters,
			HttpRequest::Parameters<std::string, UploadedFile>(
				Dict<std::string, UploadedFile>(),
				MultiValueDict<std::string, UploadedFile>()
			)
		);
	}

	void SetUp() override
	{
		this->view = new View();
	}

	void TearDown() override
	{
		delete this->view;
	}
};

TEST_F(ViewTestCase, GetTestReturnsNullptr)
{
	auto request = ViewTestCase::make_request("get");
	auto response = this->view->get(&request, nullptr);

	ASSERT_EQ(response, nullptr);

	delete response;
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
	auto expected_response = HttpResponse("");
	expected_response.set_header(
		"Allow",
		str::join(", ", std::vector<std::string>{"get", "post", "head", "options"})
	);
	expected_response.set_header("Content-Length", "0");

	auto request = ViewTestCase::make_request("options");
	auto actual_response = this->view->options(&request, nullptr);

	ASSERT_EQ(actual_response->content_type(), expected_response.content_type());
	ASSERT_EQ(actual_response->status(), expected_response.status());
	ASSERT_EQ(actual_response->charset(), expected_response.charset());

	delete actual_response;
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

	ASSERT_THROW(this->view->dispatch(nullptr), NullPointerException);

	this->view->setup(&request);
	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 200);

	delete response;
}

TEST_F(ViewTestCase, DispatchNotAllowedTest)
{
	auto request = ViewTestCase::make_request("get");
	this->view->setup(&request);

	auto response = this->view->dispatch(nullptr);

	ASSERT_EQ(response->status(), 405);

	delete response;
}

__UNIT_TESTS_END__


#endif // WASP_UNIT_TESTS_VIEWS_TESTS_VIEW_TESTS_H
