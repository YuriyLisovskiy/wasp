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

#include "../../../../src/render/internal/lexer.h"

using namespace wasp;


TEST(LexerTestCase, TestSplit)
{
	auto lexer = render::internal::lexer(
		"<html>\n{% if test %}\n<h1>{{ varvalue }}</h1>\n{% endif %}\n</html>"
	);
	auto expected = std::vector<std::string> {
		"<html>\n", "{% if test %}", "\n<h1>", "{{ varvalue }}", "</h1>\n", "{% endif %}", "\n</html>"
	};
	lexer.split();

	ASSERT_EQ(lexer.str_tokens.size(), expected.size());
	for (size_t i = 0; i < lexer.str_tokens.size(); i++)
	{
		ASSERT_EQ(lexer.str_tokens[i], expected[i]);
	}
}

/*
TEST(LexerTestCase, TestTokenize)
{
	std::string raw_string = R"(<html>
{% if test %}
	<h1>
		{% if true %}{{ varvalue }}{% endif %}
	</h1>
{% endif %}
</html>)";

	auto lexer = render::internal::lexer(raw_string);
	lexer.tokenize();

//	size_t i = 0;
}
*/
