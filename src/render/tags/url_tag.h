/**
 * render/tags/url_tag.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Render libraries.
#include <xalwart.render/syntax/parser.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../urls/pattern.h"


__RENDER_TAGS_BEGIN__

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

__RENDER_TAGS_END__
