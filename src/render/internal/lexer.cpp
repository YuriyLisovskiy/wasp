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

/**
 * An implementation of render/processors/lexer.h
 */

#include "./lexer.h"


__RENDER_INTERNAL_BEGIN__

lexer::lexer(const std::string& template_code)
{
	this->template_code = template_code;
	this->verbatim = "";
}

void lexer::split()
{
	std::smatch matches;
	std::string suffix;
	auto start = this->template_code.cbegin();
	while (std::regex_search(
		start,
		this->template_code.cend(),
		matches,
		TAG_REGEX
	))
	{
		std::string pref = matches.prefix();
		this->str_tokens.push_back(pref);
		for (const auto & match : matches)
		{
			if (match.matched)
			{
				this->str_tokens.push_back(match.str());
			}
		}

		start = matches.suffix().first;
		if (!matches.empty())
		{
			suffix = matches.suffix();
		}
	}

	if (this->str_tokens.empty())
	{
		this->str_tokens.push_back(this->template_code);
	}
	else
	{
		this->str_tokens.push_back(suffix);
	}
}

void lexer::tokenize()
{
	bool in_tag = false;
	size_t line_no = 1;
	this->split();
	for (const auto& str_token : this->str_tokens)
	{
		if (!str_token.empty())
		{
			this->create_token(str_token, line_no, in_tag);
		}

		in_tag = !in_tag;
		line_no += core::str::count(str_token, '\n');
	}
}

void lexer::create_token(
	const std::string& token_str,
	size_t line_no,
	bool in_tag,
	const std::pair<size_t, size_t>& position
)
{
	std::string block_content;
	if (in_tag && core::str::starts_with(token_str, BLOCK_TAG_START))
	{
		block_content = core::str::cut_edges(token_str, 2, 2);
		if (!this->verbatim.empty() && block_content == this->verbatim)
		{
			this->verbatim = "";
		}
	}

	if (in_tag && this->verbatim.empty())
	{
		if (core::str::starts_with(token_str, VAR_TAG_START))
		{
			this->tokens.push_back(token_t{
				token_type::var,
				core::str::trim(
					core::str::cut_edges(token_str, 2, 2)
				),
				position,
				line_no
			});
		}
		else if (core::str::starts_with(token_str, BLOCK_TAG_START))
		{
			if (core::str::rtrim(block_content.substr(0, 9)) == "verbatim")
			{
				this->verbatim = "end" + block_content;
			}

			core::str::trim(block_content);
			this->tokens.push_back(token_t{
				token_type::block, block_content, position, line_no
			});
		}
		else if (core::str::starts_with(token_str, COMMENT_TAG_START))
		{
			std::string content;
			if (token_str.find(TRANSLATOR_COMMENT_MARK) != std::string::npos)
			{
				content = core::str::cut_edges(token_str, 2, 2);
			}

			core::str::trim(content);
			this->tokens.push_back(token_t{
				token_type::comment, content, position, line_no
			});
		}
	}
	else
	{
		core::str::trim(token_str);
		this->tokens.push_back(token_t{
			token_type::text, token_str, position, line_no
		});
	}
}

__RENDER_INTERNAL_END__
