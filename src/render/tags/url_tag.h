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
 * render/tags/url_tag.h
 *
 * Purpose:
 * TODO:
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include <xalwart.render/internal/parser.h>
#include "../../urls/pattern.h"


__TAGS_BEGIN__

const std::string TAG_NAME_URL = "url";

struct url_node : public node
{
	std::shared_ptr<urls::UrlPattern> pattern;
	std::vector<std::shared_ptr<FilterExpression>> params;
	std::string var_name;

	std::string render(IContext* ctx) override;
};

extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<std::shared_ptr<urls::UrlPattern>>& patterns);

__TAGS_END__
