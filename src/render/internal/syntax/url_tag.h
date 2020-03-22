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
 * render/internal/syntax/url_tag.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// C++ libraries.
#include <string>
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../../internal/parser.h"
#include "../../internal/nodes.h"


__SYNTAX_BEGIN__

struct url_node : public node
{
	std::string text;

	explicit url_node(const std::string& var);
	std::string render(IContext* ctx) override;
};

extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<urls::UrlPattern>& patterns);

__SYNTAX_END__
