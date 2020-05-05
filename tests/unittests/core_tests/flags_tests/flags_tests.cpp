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

#include <gtest/gtest.h>

#include "../../../../src/core/flags/flag_set.h"

using namespace xw;


TEST(FlagsTestCase, ParseTest)
{
	auto fs = core::flags::FlagSet("TestFlagSet");
	auto host = fs.make_string("host", "", "");
	auto port = fs.make_long("port", 0, "");
	auto verbose = fs.make_bool("verbose", false, "");

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
	auto fs = core::flags::FlagSet("TestFlagSet");
	auto host = fs.make_string("host", "", "");
	auto port = fs.make_long("port", 0, "");
	auto verbose = fs.make_bool("verbose", true, "");

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
