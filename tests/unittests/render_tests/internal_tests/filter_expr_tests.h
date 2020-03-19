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

#ifndef WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_FILTER_EXPR_TESTS_H
#define WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_FILTER_EXPR_TESTS_H

#include <gtest/gtest.h>

#include "../../_def_.h"
#include "../../../../src/render/internal/base.h"
#include "../../../../src/render/internal/filter_expr.h"


__UNIT_TESTS_BEGIN__

TEST(FilterRegexTestCase, ParserTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "variable.attr1.attr2|default(val=\"Default value  with   spaces    !     \")|date(format='Y-m-d',tz=UTC)";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "variable");
	ASSERT_EQ(p.expression.is_const, false);

	ASSERT_EQ(p.expression.var_attrs.size(), 2);
	ASSERT_EQ(p.expression.var_attrs[0], "attr1");
	ASSERT_EQ(p.expression.var_attrs[1], "attr2");

	ASSERT_EQ(p.expression.filters.size(), 2);

	ASSERT_EQ(p.expression.filters[0].name, "default");
	ASSERT_EQ(p.expression.filters[0].args.size(), 1);
	ASSERT_EQ(p.expression.filters[0].args[0].name, "val");
	ASSERT_EQ(p.expression.filters[0].args[0].value, "\"Default value  with   spaces    !     \"");
	ASSERT_EQ(p.expression.filters[0].args[0].is_const, true);

	ASSERT_EQ(p.expression.filters[1].name, "date");
	ASSERT_EQ(p.expression.filters[1].args.size(), 2);
	ASSERT_EQ(p.expression.filters[1].args[0].name, "format");
	ASSERT_EQ(p.expression.filters[1].args[0].value, "'Y-m-d'");
	ASSERT_EQ(p.expression.filters[1].args[0].is_const, true);
	ASSERT_EQ(p.expression.filters[1].args[1].name, "tz");
	ASSERT_EQ(p.expression.filters[1].args[1].value, "UTC");
	ASSERT_EQ(p.expression.filters[1].args[1].is_const, false);
}

TEST(FilterRegexTestCase, ParseNumberParameterInFilterWithSpacesTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "variable | var_list ( val = 10 , val1 = -10 , val2 = +10 )";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "variable");
	ASSERT_EQ(p.expression.is_const, false);

	ASSERT_EQ(p.expression.var_attrs.size(), 0);

	ASSERT_EQ(p.expression.filters.size(), 1);

	ASSERT_EQ(p.expression.filters[0].name, "var_list");
	ASSERT_EQ(p.expression.filters[0].args.size(), 3);
	ASSERT_EQ(p.expression.filters[0].args[0].name, "val");
	ASSERT_EQ(p.expression.filters[0].args[0].value, "10");
	ASSERT_EQ(p.expression.filters[0].args[0].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[1].name, "val1");
	ASSERT_EQ(p.expression.filters[0].args[1].value, "-10");
	ASSERT_EQ(p.expression.filters[0].args[1].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[2].name, "val2");
	ASSERT_EQ(p.expression.filters[0].args[2].value, "+10");
	ASSERT_EQ(p.expression.filters[0].args[2].is_const, true);
}

TEST(FilterRegexTestCase, ParseNumberParameterInFilterNoSpacesTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "variable|var_list(val=10,val1=-10,val2=+10)";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "variable");
	ASSERT_EQ(p.expression.is_const, false);

	ASSERT_EQ(p.expression.var_attrs.size(), 0);

	ASSERT_EQ(p.expression.filters.size(), 1);

	ASSERT_EQ(p.expression.filters[0].name, "var_list");
	ASSERT_EQ(p.expression.filters[0].args.size(), 3);
	ASSERT_EQ(p.expression.filters[0].args[0].name, "val");
	ASSERT_EQ(p.expression.filters[0].args[0].value, "10");
	ASSERT_EQ(p.expression.filters[0].args[0].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[1].name, "val1");
	ASSERT_EQ(p.expression.filters[0].args[1].value, "-10");
	ASSERT_EQ(p.expression.filters[0].args[1].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[2].name, "val2");
	ASSERT_EQ(p.expression.filters[0].args[2].value, "+10");
	ASSERT_EQ(p.expression.filters[0].args[2].is_const, true);
}

TEST(FilterRegexTestCase, ParseNumberParameterInFilterStandardNotationTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "variable | var_list(val=10, val1=-10, val2=+10)";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "variable");
	ASSERT_EQ(p.expression.is_const, false);

	ASSERT_EQ(p.expression.var_attrs.size(), 0);

	ASSERT_EQ(p.expression.filters.size(), 1);

	ASSERT_EQ(p.expression.filters[0].name, "var_list");
	ASSERT_EQ(p.expression.filters[0].args.size(), 3);
	ASSERT_EQ(p.expression.filters[0].args[0].name, "val");
	ASSERT_EQ(p.expression.filters[0].args[0].value, "10");
	ASSERT_EQ(p.expression.filters[0].args[0].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[1].name, "val1");
	ASSERT_EQ(p.expression.filters[0].args[1].value, "-10");
	ASSERT_EQ(p.expression.filters[0].args[1].is_const, true);
	ASSERT_EQ(p.expression.filters[0].args[2].name, "val2");
	ASSERT_EQ(p.expression.filters[0].args[2].value, "+10");
	ASSERT_EQ(p.expression.filters[0].args[2].is_const, true);
}

TEST(FilterRegexTestCase, ParseDoubleValueTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "123.321";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "123.321");
	ASSERT_EQ(p.expression.is_const, true);

	ASSERT_EQ(p.expression.var_attrs.size(), 0);
	ASSERT_EQ(p.expression.filters.size(), 0);
}

TEST(FilterRegexTestCase, ParseIntValueTest)
{
	using render::internal::expression_parser;
	render::internal::token_t token;
	token.content = "123";

	expression_parser p{token};
	p.parse();

	ASSERT_EQ(p.expression.var_name, "123");
	ASSERT_EQ(p.expression.is_const, true);

	ASSERT_EQ(p.expression.var_attrs.size(), 0);
	ASSERT_EQ(p.expression.filters.size(), 0);
}

__UNIT_TESTS_END__

#endif // WASP_UNIT_TESTS_RENDER_TESTS_INTERNAL_TESTS_FILTER_EXPR_TESTS_H
