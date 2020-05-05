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

#include <fstream>

#include <gtest/gtest.h>

#include "../../../../src/core/path.h"
#include "../../../../src/core/exceptions.h"
#include "../../../../src/core/string.h"
#include "../../../../src/core/parsers/multipart_parser.h"
#include "../../../../src/http/request.h"

using namespace xw;


class MultipartParserStaticTestCase : public ::testing::Test
{
protected:
	const std::string CONTENT_TYPE = "multipart/form-data; boundary=------WebKitFormBoundaryzY2033Pw5Bz7HHAG";
	const std::string ORIG_BOUNDARY = "------WebKitFormBoundaryzY2033Pw5Bz7HHAG";
	const std::string DIFFERENT_BOUNDARY = "------WebKitFormBoundaryzY2033Pw5Bz7HHAv";
};

TEST_F(MultipartParserStaticTestCase, AssertBoundaryTestSuccess)
{
	ASSERT_NO_THROW(core::internal::multipart_parser::assert_boundary(
		this->ORIG_BOUNDARY,
		this->ORIG_BOUNDARY
	));
}

TEST_F(MultipartParserStaticTestCase, AssertBoundaryTestThrows)
{
	ASSERT_THROW(core::internal::multipart_parser::assert_boundary(
		this->ORIG_BOUNDARY,
		this->DIFFERENT_BOUNDARY
	), core::MultiPartParserError);
}

TEST_F(MultipartParserStaticTestCase, AssertGetBoundaryTestSuccess)
{
	ASSERT_EQ(
		core::internal::multipart_parser::get_boundary(this->CONTENT_TYPE),
		core::str::ltrim(this->ORIG_BOUNDARY, "-")
	);
}

TEST_F(MultipartParserStaticTestCase, AssertGetBoundaryTestThrowsInvalidContentType)
{
	ASSERT_THROW(
		core::internal::multipart_parser::get_boundary("application/x-www-urlencoded"),
		core::MultiPartParserError
	);
}

TEST_F(MultipartParserStaticTestCase, AssertGetBoundaryTestThrowsEmptyBoundary)
{
	ASSERT_THROW(
		core::internal::multipart_parser::get_boundary("multipart/form-data"),
		core::MultiPartParserError
	);
}


class MultipartParserTestCase : public ::testing::Test
{
protected:
	const std::string CONTENT_TYPE = "multipart/form-data; boundary=------WebKitFormBoundaryzY2033Pw5Bz7HHAG";

	const std::string BODY_WITH_SINGLE_PARAM =
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"mail\"\r\n\r\n"
		"some_mail@gmail.com\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG--";

	const std::string BODY_WITH_MULTIPLE_PARAM =
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"mail\"\r\n\r\n"
		"some_mail@gmail.com\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"name\"\r\n\r\n"
		"Xavier\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"birth_year\"\r\n\r\n"
		"2000\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG--";

	const std::string FILE_CONTENT =
		"male(xavier).     % 1st generation\n"
		"male(darren).     % 2nd generation\n"
		"male(henry).      % 3rd generation\n"
		"male(ronald).     % 3rd generation\n\n"
		"male(ted).        % 4th generation\n"
		"male(dexter).     % 4th generation\n"
		"male(caleb).      % 4th generation\n\n"
		"male(alan).       % 5th generation\n"
		"male(bernard).    % 5th generation\n\n"
		"female(louisa).   % 2nd generation\n"
		"female(patricia). % 2nd generation\n\n"
		"female(isabelle). % 3rd generation\n\n"
		"female(caroline). % 4th generation\n"
		"female(ashley).   % 4th generation\n"
		"female(alice).    % 4th generation\n\n"
		"female(eva).      % 5th generation\n"
		"female(annabel).  % 5th generation\n\n"
		"% parent(MotherOrFather, Child).\n"
		"parent(louisa, xavier).\n"
		"parent(darren, xavier).\n\n"
		"parent(isabelle, patricia).\n"
		"parent(henry, patricia).\n"
		"parent(isabelle, louisa).\n"
		"parent(henry, louisa).\n\n"
		"parent(ted, isabelle).\n"
		"parent(caroline, isabelle).\n"
		"parent(caleb, henry).\n"
		"parent(alice, henry).\n"
		"parent(caleb, ronald).\n"
		"parent(alice, ronald).\n\n"
		"parent(alan, ted).\n"
		"parent(eva, ted).\n"
		"parent(bernard, caroline).\n"
		"parent(annabel, caroline).\n"
		"parent(bernard, dexter).\n"
		"parent(annabel, dexter).\n"
		"parent(bernard, ashley).\n"
		"parent(annabel, ashley).\n\n"
		"% rules\n"
		"mother(Mother, Child) :- female(Mother), parent(Mother, Child).\n"
		"father(Father, Child) :- male(Father), parent(Father, Child).\n\n"
		"siblings(Person1, Person2) :-\n"
		"\tmother(Mother, Person1), mother(Mother, Person2),\n"
		"\tfather(Father, Person1), father(Father, Person2),\n"
		"\tPerson1 \\= Person2.\n\n"
		"brother(Brother, Child) :- siblings(Brother, Child), male(Brother).\n"
		"sister(Sister, Child) :- siblings(Sister, Child), female(Sister).\n\n"
		"descendant(Young, Old) :-\n"
		"\tparent(Old, Young), female(Young);\n"
		"\tparent(Old, Young), male(Young).\n"
		"descendant(Young, Old) :- parent(Old, Parent), descendant(Young, Parent).\n\n"
		"ancestor(Old, Young) :-\n"
		"\tparent(Old, Young), female(Old);\n"
		"\tparent(Old, Young), male(Old).\n"
		"ancestor(Old, Young) :- parent(Old, Parent), ancestor(Parent, Young).\n\n"
		"grandparent(GrandParent, GrandChild) :-\n"
		"\tparent(GrandParent, Parent),\n"
		"\tparent(Parent, GrandChild).\n\n"
		"uncle(Uncle, Person) :-\n"
		"\tmother(Mother, Person), brother(Uncle, Mother);\n"
		"\tfather(Father, Person), brother(Uncle, Father).\n\n"
		"aunt(Uncle, Person) :-\n"
		"\tmother(Mother, Person), sister(Uncle, Mother);\n"
		"\tfather(Father, Person), sister(Uncle, Father).\n";

	const std::string BODY_WITH_FILE_AND_STR_PARAM =
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"super_file\"; filename=\"FamilyTree.pro\"\r\n"
		"Content-Type: application/octet-stream\r\n\r\n"
		+ FILE_CONTENT + "\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"mail\"\r\n\r\n"
		"some_mail@gmail.com\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"name\"\r\n\r\n"
		"Xavier\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG\r\n"
		"Content-Disposition: form-data; name=\"birth_year\"\r\n\r\n"
		"2000\r\n"
		"------WebKitFormBoundaryzY2033Pw5Bz7HHAG--";

	const std::string ROOT = core::path::cwd() + "/";

	core::internal::multipart_parser* parser = nullptr;

	void SetUp() override
	{
		this->parser = new core::internal::multipart_parser(this->ROOT);
	}

	void TearDown() override
	{
		delete this->parser;
	}
};

TEST_F(MultipartParserTestCase, ParseSingleParameterTest)
{
	this->parser->parse(this->CONTENT_TYPE, this->BODY_WITH_SINGLE_PARAM);
	auto params = http::HttpRequest::Parameters(
		this->parser->post_values, this->parser->multi_post_value
	);

	ASSERT_EQ(params.size(), 1);
	ASSERT_TRUE(params.contains("mail"));
	ASSERT_EQ(params.get("mail"), "some_mail@gmail.com");

	auto file_params = http::HttpRequest::Parameters(
		this->parser->file_values, this->parser->multi_file_value
	);
	ASSERT_EQ(file_params.size(), 0);
}

TEST_F(MultipartParserTestCase, ParseMultipleParametersWithoutFilesTest)
{
	this->parser->parse(this->CONTENT_TYPE, this->BODY_WITH_MULTIPLE_PARAM);
	auto params = http::HttpRequest::Parameters(
		this->parser->post_values, this->parser->multi_post_value
	);

	ASSERT_EQ(params.size(), 3);
	ASSERT_TRUE(params.contains("mail"));
	ASSERT_EQ(params.get("mail"), "some_mail@gmail.com");
	ASSERT_TRUE(params.contains("name"));
	ASSERT_EQ(params.get("name"), "Xavier");
	ASSERT_TRUE(params.contains("birth_year"));
	ASSERT_EQ(params.get("birth_year"), "2000");

	auto file_params = http::HttpRequest::Parameters(
		this->parser->file_values, this->parser->multi_file_value
	);
	ASSERT_EQ(file_params.size(), 0);
}

TEST_F(MultipartParserTestCase, ParseMultipleParametersWithFilesTest)
{
	this->parser->parse(this->CONTENT_TYPE, this->BODY_WITH_FILE_AND_STR_PARAM);
	auto post = http::HttpRequest::Parameters(
		this->parser->post_values, this->parser->multi_post_value
	);

	ASSERT_EQ(post.size(), 3);
	ASSERT_TRUE(post.contains("mail"));
	ASSERT_EQ(post.get("mail"), "some_mail@gmail.com");
	ASSERT_TRUE(post.contains("name"));
	ASSERT_EQ(post.get("name"), "Xavier");
	ASSERT_TRUE(post.contains("birth_year"));
	ASSERT_EQ(post.get("birth_year"), "2000");

	auto files = http::HttpRequest::Parameters(
		this->parser->file_values, this->parser->multi_file_value
	);

	ASSERT_EQ(files.size(), 1);
	ASSERT_TRUE(files.contains("super_file"));

	auto super_file = files.get("super_file");
	super_file.save();

	ASSERT_EQ(super_file.size(), this->FILE_CONTENT.size());
	ASSERT_EQ(super_file.name(), this->ROOT + "FamilyTree.pro");
	ASSERT_EQ(super_file.boundary(), "WebKitFormBoundaryzY2033Pw5Bz7HHAG");
	ASSERT_TRUE(super_file.exists());
	ASSERT_EQ(super_file.charset(), "");
	ASSERT_EQ(super_file.content_disposition(), "form-data");
	ASSERT_EQ(super_file.content_type(), "application/octet-stream");

	std::ifstream file_stream(this->ROOT + "FamilyTree.pro");
	std::string actual_file_content(
		(std::istreambuf_iterator<char>(file_stream)),
		std::istreambuf_iterator<char>()
	);
	file_stream.close();

	ASSERT_EQ(actual_file_content, this->FILE_CONTENT);

	if (std::remove((this->ROOT + "FamilyTree.pro").c_str()) != 0)
	{
		ASSERT_TRUE(false);
	}
}
