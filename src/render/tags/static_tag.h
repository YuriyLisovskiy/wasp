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

inline const char* TAG_NAME_STATIC = "static";
inline const char* TAG_NAME_MEDIA = "media";

// TESTME: static_node
// TODO: docs for 'static_node'
struct StaticNode : public syntax::Node
{
	std::string prefix;
	std::string var_name;
	std::shared_ptr<syntax::FilterExpression> path;

	std::string render(abc::IContext* ctx) override;
};

// TESTME: make_static_tag
// TODO: docs for 'make_static_tag'
extern std::function<std::shared_ptr<syntax::Node>(
	syntax::Parser*, syntax::Token& token
)> make_static_tag(const std::string& name, const std::string& prefix);

__RENDER_TAGS_END__
