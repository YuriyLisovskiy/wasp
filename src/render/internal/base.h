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
 * render/processors/base.h
 * Purpose:
 *
 * TODO: write docs for render/processors/base.h
 */

#pragma once

// C++ libraries.
#include <string>
#include <vector>
#include <memory>
#include <regex>

// Module definitions.
#include "../_def_.h"

// Wasp libraries.
#include "../base.h"
#include "../../core/regex.h"
#include "../../core/object/object.h"
#include "../../core/types/value.h"
#include "../../core/string/str.h"

__RENDER_INTERNAL_BEGIN__

const std::string BLOCK_TAG_START = "{%";
const std::string BLOCK_TAG_END = "%}";
const std::string VAR_TAG_START = "{{";
const std::string VAR_TAG_END = "}}";
const std::string COMMENT_TAG_START = "{#";
const std::string COMMENT_TAG_END = "#}";
const std::string TRANSLATOR_COMMENT_MARK = "Translators";

const char VAR_ATTR_SEP = '.';
const char FILTER_SEP = '|';
const char FILTER_ARG_SEP = ',';
const char FILTER_ARG_NAME_VAL_SEP = '=';


class Variable final
{
private:
	std::string _content;
	bool _is_constant;

	static std::regex number_regex;

public:
	Variable();
	explicit Variable(
		const std::string& content,
		const std::vector<std::string>& attributes,
		bool is_constant
	);

	std::shared_ptr<core::object::Object> resolve(
		IContext* ctx
	);
};

__RENDER_INTERNAL_END__
