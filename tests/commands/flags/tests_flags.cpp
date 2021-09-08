/**
 * commands/flags/tests_flags.cpp
 *
 * Copyright (c) 2019, 2021 Yuriy Lisovskiy
 */

#include <gtest/gtest.h>

#include "../../../src/commands/flags/flag_set.h"

using namespace xw;


TEST(FlagsTestCase, ParseTest)
{
	auto fs = cmd::flags::FlagSet("TestFlagSet");
	auto host = fs.make_string("h", "host", "", "");
	auto port = fs.make_long("p", "port", 0, "");
	auto verbose = fs.make_bool("v", "verbose", false, "");

	std::vector<std::string> arguments = {"ignored", "--host", "127.0.0.1", "--port", "8000"};
	std::vector<char*> argv;
	for (const auto& arg : arguments)
	{
		argv.push_back((char*)arg.data());
	}

	argv.push_back(nullptr);
	fs.parse((int)argv.size() - 1, argv.data());

	ASSERT_EQ(host->get(), "127.0.0.1");
	ASSERT_EQ(port->get(), 8000);
	ASSERT_EQ(verbose->get(), false);
}

TEST(FlagsTestCase, ParseWithEqSignsTest)
{
	auto fs = cmd::flags::FlagSet("TestFlagSet");
	auto host = fs.make_string("h", "host", "", "");
	auto port = fs.make_long("p", "port", 0, "");
	auto verbose = fs.make_bool("p", "verbose", true, "");

	std::vector<std::string> arguments = {"ignored", "--host=127.0.0.1", "--port", "8000", "--verbose=false"};
	std::vector<char*> argv;
	for (const auto& arg : arguments)
	{
		argv.push_back((char*)arg.data());
	}

	argv.push_back(nullptr);
	fs.parse((int)argv.size() - 1, argv.data());

	ASSERT_EQ(host->get(), "127.0.0.1");
	ASSERT_EQ(port->get(), 8000);
	ASSERT_EQ(verbose->get(), false);
}
