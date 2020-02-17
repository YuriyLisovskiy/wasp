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

#ifndef WASP_UNIT_TESTS_RENDER_TESTS_PROCESSORS_TESTS_FILTER_EXPR_TESTS_H
#define WASP_UNIT_TESTS_RENDER_TESTS_PROCESSORS_TESTS_FILTER_EXPR_TESTS_H

#include <gtest/gtest.h>

#include "../../_def_.h"
#include "../../../../src/render/processors/filter_expr.h"


__UNIT_TESTS_BEGIN__

TEST(FilterRegexTestCase, SearchTest)
{
	core::rgx::ArgRegex filter_regex(render::internal::FILTER_REGEX);
	filter_regex.search_iter("variable|default(val=\"Default value\")");
	while (filter_regex.next())
	{
		auto gr = filter_regex.groups();
		size_t i = gr.size();
	}
}

__UNIT_TESTS_END__

#endif // WASP_UNIT_TESTS_RENDER_TESTS_PROCESSORS_TESTS_FILTER_EXPR_TESTS_H
