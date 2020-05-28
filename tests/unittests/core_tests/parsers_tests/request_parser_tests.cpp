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
#include <string>
#include <algorithm>

#include <gtest/gtest.h>

#include "../../../../src/core/parsers/request_parser.h"

using namespace xw;


TEST(RequestParserTestCase, IsCharTest)
{
	for (size_t i = 0; i < 128; i++)
	{
		ASSERT_TRUE(core::internal::request_parser::is_char(i));
	}
}

TEST(RequestParserTestCase, IsNotCharTest)
{
	ASSERT_FALSE(core::internal::request_parser::is_char(-1));
	ASSERT_FALSE(core::internal::request_parser::is_char(128));
}

TEST(RequestParserTestCase, IsControlTest)
{
	for (size_t i = 0; i < 32; i++)
	{
		ASSERT_TRUE(core::internal::request_parser::is_control(i));
	}

	ASSERT_TRUE(core::internal::request_parser::is_control(127));
}

TEST(RequestParserTestCase, IsNotControlTest)
{
	ASSERT_FALSE(core::internal::request_parser::is_control(-1));

	for (size_t i = 32; i < 127; i++)
	{
		ASSERT_FALSE(core::internal::request_parser::is_control(i));
	}

	ASSERT_FALSE(core::internal::request_parser::is_control(128));
}

TEST(RequestParserTestCase, IsSpecialTest)
{
	char specials[] = {
			'(', ')', '<', '>', '@', ',', ';', ':', '"', '/', '\\', '[', ']', '?', '=', '{', '}', '\t'
	};
	for (auto special : specials)
	{
		ASSERT_TRUE(core::internal::request_parser::is_special(special));
	}
}

TEST(RequestParserTestCase, IsNotSpecialTest)
{
	ASSERT_FALSE(core::internal::request_parser::is_special('f'));
	ASSERT_FALSE(core::internal::request_parser::is_special('0'));
	ASSERT_FALSE(core::internal::request_parser::is_special('\n'));
}

TEST(RequestParserTestCase, IsDigitTest)
{
	for (char i = '0'; i <= '9'; i++)
	{
		ASSERT_TRUE(core::internal::request_parser::is_digit(i));
	}
}

TEST(RequestParserTestCase, IsNotDigitTest)
{
	ASSERT_FALSE(core::internal::request_parser::is_digit('h'));
	ASSERT_FALSE(core::internal::request_parser::is_digit('e'));
	ASSERT_FALSE(core::internal::request_parser::is_digit('l'));
	ASSERT_FALSE(core::internal::request_parser::is_digit('l'));
	ASSERT_FALSE(core::internal::request_parser::is_digit('o'));
}


class ParseRequestTestCase : public ::testing::Test
{
protected:
	core::internal::request_parser* parser{};
	const std::string HEADERS = "POST /hello/world HTTP/1.1\r\n"
								"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
								"Host: 127.0.0.1:8000\r\n"
								"Content-Type: application/x-www-form-urlencoded\r\n"
								"Content-Length: 38\r\n"
								"Accept-Language: en-us\r\n"
								"Accept-Encoding: gzip, deflate\r\n"
								"Connection: Keep-Alive\r\n"
								"\r\n";
	const size_t VERSION_MAJOR = 1;
	const size_t VERSION_MINOR = 1;
	const std::string METHOD = "POST";
	const std::string PATH = "/hello/world";
	const size_t HEADERS_COUNT = 7;
	const std::pair<std::string, std::string> USER_AGENT{"User-Agent", "Mozilla/4.0 (compatible; MSIE5.01; Windows NT)"};
	const std::pair<std::string, std::string> HOST{"Host", "127.0.0.1:8000"};
	const std::pair<std::string, std::string> CONTENT_TYPE{"Content-Type", "application/x-www-form-urlencoded"};
	const std::pair<std::string, std::string> CONTENT_LENGTH{"Content-Length", "38"};
	const std::pair<std::string, std::string> ACCEPT_LANGUAGE{"Accept-Language", "en-us"};
	const std::pair<std::string, std::string> ACCEPT_ENCODING{"Accept-Encoding", "gzip, deflate"};
	const std::pair<std::string, std::string> CONNECTION{"Connection", "Keep-Alive"};

	const std::string BODY = "hello=world&content=string&params=text";
	const size_t PARAMS_COUNT = 3;
	const std::pair<std::string, std::string> PARAM_HELLO{"hello", "world"};
	const std::pair<std::string, std::string> PARAM_CONTENT{"content", "string"};
	const std::pair<std::string, std::string> PARAM_PARAMS{"params", "text"};

	const std::string FORM_DATA_HEADERS = "POST /hello/world HTTP/1.1\r\n"
	                            "User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\r\n"
	                            "Host: 127.0.0.1:8000\r\n"
	                            "Content-Type: multipart/form-data; boundary=------WebKitFormBoundary7N2PL3etPfSwBAXr\r\n"
	                            "Content-Length: 328\r\n"
	                            "Accept-Language: en-us\r\n"
	                            "Accept-Encoding: gzip, deflate\r\n"
	                            "Connection: Keep-Alive\r\n"
	                            "\r\n";
	const std::string FORM_DATA_BODY = "------WebKitFormBoundary7N2PL3etPfSwBAXr\r\n"
									"Content-Disposition: form-data; name=\"super_file\"; filename=\"hello_world.txt\"\r\n"
									"Content-Type: application/octet-stream\r\n"
									"\r\n"
									"Hello, World!\r\n"
									"------WebKitFormBoundary7N2PL3etPfSwBAXr\r\n"
									"Content-Disposition: form-data; name=\"mail\"\r\n"
									"\r\n"
									"email@gmail.com\r\n"
									"------WebKitFormBoundary7N2PL3etPfSwBAXr--\r\n";
	const size_t FORM_DATA_POST_COUNT = 1;
	const size_t FORM_DATA_FILES_COUNT = 1;
	const std::pair<std::string, std::string> FORM_DATA_CONTENT_LENGTH{"Content-Length", "328"};
	const std::pair<std::string, std::string> FORM_DATA_CONTENT_TYPE{"Content-Type", "multipart/form-data; boundary=------WebKitFormBoundary7N2PL3etPfSwBAXr"};
	const std::pair<std::string, std::string> MAIL{"mail", "email@gmail.com"};

	void SetUp() override
	{
		this->parser = new core::internal::request_parser();
	}

	void TearDown() override
	{
		delete this->parser;
	}
};

TEST_F(ParseRequestTestCase, ParseXWWWUrlencodedTest)
{
	this->parser->parse_headers(this->HEADERS);
	auto headers = this->parser->headers;

	ASSERT_EQ(headers.size(), this->HEADERS_COUNT);

	ASSERT_EQ(this->parser->method, this->METHOD);
	ASSERT_EQ(this->parser->major_v, this->VERSION_MAJOR);
	ASSERT_EQ(this->parser->minor_v, this->VERSION_MINOR);
	ASSERT_EQ(this->parser->path, this->PATH);

	ASSERT_TRUE(headers.find(this->USER_AGENT.first) != headers.end());
	ASSERT_EQ(headers[this->USER_AGENT.first], this->USER_AGENT.second);

	ASSERT_TRUE(headers.find(this->CONTENT_TYPE.first) != headers.end());
	ASSERT_EQ(headers[this->CONTENT_TYPE.first], this->CONTENT_TYPE.second);

	ASSERT_TRUE(headers.find(this->CONTENT_LENGTH.first) != headers.end());
	ASSERT_EQ(headers[this->CONTENT_LENGTH.first], this->CONTENT_LENGTH.second);

	ASSERT_TRUE(headers.find(this->HOST.first) != headers.end());
	ASSERT_EQ(headers[this->HOST.first], this->HOST.second);

	ASSERT_TRUE(headers.find(this->ACCEPT_LANGUAGE.first) != headers.end());
	ASSERT_EQ(headers[this->ACCEPT_LANGUAGE.first], this->ACCEPT_LANGUAGE.second);

	ASSERT_TRUE(headers.find(this->ACCEPT_ENCODING.first) != headers.end());
	ASSERT_EQ(headers[this->ACCEPT_ENCODING.first], this->ACCEPT_ENCODING.second);

	ASSERT_TRUE(headers.find(this->CONNECTION.first) != headers.end());
	ASSERT_EQ(headers[this->CONNECTION.first], this->CONNECTION.second);

	this->parser->parse_body(this->BODY, "");

	ASSERT_EQ(this->parser->content, this->BODY);
	ASSERT_EQ(this->parser->post_parameters.size(), this->PARAMS_COUNT);
	ASSERT_EQ(this->BODY.size(),std::stoi(headers[this->CONTENT_LENGTH.first]));

	ASSERT_TRUE(this->parser->post_parameters.contains(this->PARAM_HELLO.first));
	ASSERT_EQ(this->parser->post_parameters.get(this->PARAM_HELLO.first), this->PARAM_HELLO.second);

	ASSERT_TRUE(this->parser->post_parameters.contains(this->PARAM_CONTENT.first));
	ASSERT_EQ(this->parser->post_parameters.get(this->PARAM_CONTENT.first), this->PARAM_CONTENT.second);

	ASSERT_TRUE(this->parser->post_parameters.contains(this->PARAM_PARAMS.first));
	ASSERT_EQ(this->parser->post_parameters.get(this->PARAM_PARAMS.first), this->PARAM_PARAMS.second);
}

TEST_F(ParseRequestTestCase, ParseFormDataTest)
{
	this->parser->parse_headers(this->FORM_DATA_HEADERS);
	auto headers = this->parser->headers;

	ASSERT_EQ(headers.size(), this->HEADERS_COUNT);

	ASSERT_EQ(this->parser->method, this->METHOD);
	ASSERT_EQ(this->parser->major_v, this->VERSION_MAJOR);
	ASSERT_EQ(this->parser->minor_v, this->VERSION_MINOR);
	ASSERT_EQ(this->parser->path, this->PATH);

	ASSERT_TRUE(headers.find(this->USER_AGENT.first) != headers.end());
	ASSERT_EQ(headers[this->USER_AGENT.first], this->USER_AGENT.second);

	ASSERT_TRUE(headers.find(this->FORM_DATA_CONTENT_TYPE.first) != headers.end());
	ASSERT_EQ(headers[this->FORM_DATA_CONTENT_TYPE.first], this->FORM_DATA_CONTENT_TYPE.second);

	ASSERT_TRUE(headers.find(this->FORM_DATA_CONTENT_LENGTH.first) != headers.end());
	ASSERT_EQ(headers[this->FORM_DATA_CONTENT_LENGTH.first], this->FORM_DATA_CONTENT_LENGTH.second);

	ASSERT_TRUE(headers.find(this->HOST.first) != headers.end());
	ASSERT_EQ(headers[this->HOST.first], this->HOST.second);

	ASSERT_TRUE(headers.find(this->ACCEPT_LANGUAGE.first) != headers.end());
	ASSERT_EQ(headers[this->ACCEPT_LANGUAGE.first], this->ACCEPT_LANGUAGE.second);

	ASSERT_TRUE(headers.find(this->ACCEPT_ENCODING.first) != headers.end());
	ASSERT_EQ(headers[this->ACCEPT_ENCODING.first], this->ACCEPT_ENCODING.second);

	ASSERT_TRUE(headers.find(this->CONNECTION.first) != headers.end());
	ASSERT_EQ(headers[this->CONNECTION.first], this->CONNECTION.second);

	this->parser->parse_body(this->FORM_DATA_BODY, "");

	ASSERT_EQ(this->parser->content, this->FORM_DATA_BODY);
	ASSERT_EQ(this->parser->post_parameters.size(), this->FORM_DATA_POST_COUNT);
	ASSERT_EQ(this->parser->files_parameters.size(), this->FORM_DATA_FILES_COUNT);
	ASSERT_EQ(this->FORM_DATA_BODY.size(),std::stoi(headers[this->FORM_DATA_CONTENT_LENGTH.first]));

	ASSERT_TRUE(this->parser->post_parameters.contains(this->MAIL.first));
	ASSERT_EQ(this->parser->post_parameters.get(this->MAIL.first), this->MAIL.second);
}
