/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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

#include "../../../../src/render/internal/parser.h"

using namespace wasp;


TEST(ParserTestCase, TestSplit)
{
	using namespace wasp::render::internal;
	ASSERT_EQ(parser::get_command("hello()"), "hello");
	ASSERT_EQ(parser::get_command("_hello()"), "_hello");
	ASSERT_EQ(parser::get_command("_h3e_3llo_()"), "_h3e_3llo_");
}
