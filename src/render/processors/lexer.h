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
 * Tokenizer of default Wasp's render engine.
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
#include "../../core/string/str.h"
#include "./token.h"


__RENDER_INTERNAL_BEGIN__

const std::string BLOCK_TAG_START = "{%";
const std::string BLOCK_TAG_END = "%}";
const std::string VAR_TAG_START = "{{";
const std::string VAR_TAG_END = "}}";
const std::string COMMENT_TAG_START = "{#";
const std::string COMMENT_TAG_END = "#}";
const std::string VAR_ATTR_SEP = ".";
const std::string TRANSLATOR_COMMENT_MARK = "Translators";

using Regex = core::rgx::Regex;
const std::regex TAG_REGEX = std::regex(
	"" + Regex::escape(BLOCK_TAG_START) + ".*?" + Regex::escape(BLOCK_TAG_END) + "|"
	+ Regex::escape(VAR_TAG_START) + ".*?" + Regex::escape(VAR_TAG_END) + "|"
	+ Regex::escape(COMMENT_TAG_START) + ".*?" + Regex::escape(COMMENT_TAG_END) + ""
);

struct lexer
{
	std::string verbatim;
	std::string template_code;
	std::vector<std::string> str_tokens;
	std::vector<token_t> tokens;

	lexer(const std::string& template_code);
	void split();
	void tokenize();

	/// Convert the given token string into a new token object and return it.
	///
	/// If in_tag is true, we are processing something that matched a tag,
	/// otherwise it should be treated as a literal string.
	void create_token(
		const std::string& token_str,
		size_t line_no,
		bool in_tag,
		const std::pair<size_t, size_t>& position = {}
	);
};

__RENDER_INTERNAL_END__
