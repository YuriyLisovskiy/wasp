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
const std::string TRANSLATOR_COMMENT_MARK = "Translators";

const std::string FILTER_SEP = "|";
const std::string FILTER_ARG_SEP = ":";

using re = core::rgx::Regex;

const std::string I18NOpen = re::escape("_(");
const std::string I18NClose = re::escape(")");
const std::string SQ_STR = R"('[^'\\]*(?:\\.[^'\\]*)*')";
const std::string DQ_STR = R"("[^"\\]*(?:\\.[^"\\]*)*")";

const std::string CONST_STRING = "(?:" + I18NOpen + DQ_STR + I18NClose + "|" +
	I18NOpen + SQ_STR + I18NClose + "|" + DQ_STR + "|" + SQ_STR + ")";

__RENDER_INTERNAL_END__
