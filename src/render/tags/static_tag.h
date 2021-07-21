/**
 * render/tags/static_tag.h
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


__RENDER_TAGS_BEGIN__

const std::string TAG_NAME_STATIC = "static";
const std::string TAG_NAME_MEDIA = "media";

struct static_node : public node
{
	std::string prefix;
	std::string var_name;
	std::shared_ptr<FilterExpression> path;

	std::string render(abc::IContext* ctx) override;
};

extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_static_tag(const std::string& name, const std::string& prefix);

__RENDER_TAGS_END__
