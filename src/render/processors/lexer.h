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
 * render/processors/lexer.h
 *
 * Purpose:
 * TODO: write docs for render/processors/lexer.h
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <regex>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../../core/regex.h"


__RENDER_INTERNAL_BEGIN__

const std::string BLOCK_TAG_START = "{%";
const std::string BLOCK_TAG_END = "%}";
const std::string VAR_TAG_START = "{{";
const std::string VAR_TAG_END = "}}";
const std::string COMMENT_TAG_START = "{#";
const std::string COMMENT_TAG_END = "#}";
const std::string VAR_ATTR_SEP = ".";

enum token_type
{
	text,
	block,
	var,
	comment
};

struct lexer
{
	using Regex = core::rgx::Regex;
	std::regex tag_regex;

	lexer()
	{
		std::string any_or_empty = ".*?";
		this->tag_regex = std::regex(
			"" + Regex::escape(BLOCK_TAG_START) + any_or_empty + Regex::escape(BLOCK_TAG_END) + "|"
			+ Regex::escape(VAR_TAG_START) + any_or_empty + Regex::escape(VAR_TAG_END) + "|"
			+ Regex::escape(COMMENT_TAG_START) + any_or_empty + Regex::escape(COMMENT_TAG_END) + ""
		);
	}

	std::vector<std::string> split(const std::string& template_code)
	{
		std::vector<std::string> result;
		std::smatch prev_matches, matches;
		auto start = template_code.cbegin();
		while (std::regex_search(
			start,
			template_code.cend(),
			matches,
			this->tag_regex
		))
		{
			std::string pref = matches.prefix();
			result.push_back(pref);
			for (const auto & match : matches)
			{
				if (match.matched)
				{
					result.push_back(match.str());
				}
			}

			start = matches.suffix().first;
			prev_matches = matches;
		}

		result.push_back(prev_matches.suffix());
		return result;
	}
};

struct token
{

};

__RENDER_INTERNAL_END__
