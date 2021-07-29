/**
 * core/flags/tests_parser.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/core/flags/parser.h"

using namespace xw;


TEST(TestCase_args_parser, parseValid)
{
	std::vector<std::string> arguments = {"./program", "--host=localhost", "--retries", "5", "--ports", "1,2,3,4,5"};
	std::vector<char*> argv;
	argv.reserve(arguments.size());
	for (const auto& arg : arguments)
	{
		argv.push_back((char*)arg.data());
	}

	argv.push_back(nullptr);
	core::flags::internal::args_parser parser((int)argv.size() - 1, argv.data(), 1, false);

	std::map<std::string, std::string> expected = {
		{"host", "localhost"}, {"retries", "5"}, {"ports", "1,2,3,4,5"}
	};
	ASSERT_EQ(expected.size(), parser.flags.size());
	for (const auto& item : expected)
	{
		ASSERT_EQ(parser.flags[item.first], item.second);
	}
}

TEST(TestCase_args_parser, parseInvalidValid)
{
	std::vector<std::string> arguments = {"--host=localhost", "--retries", "5", "ports", "1,2,3,4,5"};
	std::vector<char*> argv;
	argv.reserve(arguments.size());
	for (const auto& arg : arguments)
	{
		argv.push_back((char*)arg.data());
	}

	argv.push_back(nullptr);

	core::flags::internal::args_parser parser((int)argv.size() - 1, argv.data(), 0, false);
	std::map<std::string, std::string> expected = {
		{"host", "localhost"}, {"retries", "1,2,3,4,5"}
	};
	ASSERT_EQ(expected.size(), parser.flags.size());
	for (const auto& item : expected)
	{
		ASSERT_EQ(parser.flags[item.first], item.second);
	}
}
