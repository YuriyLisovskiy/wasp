/**
 * render/tags/url_tag.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Render libraries.
#include <xalwart.render/syntax/parser.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../urls/abc.h"


__RENDER_TAGS_BEGIN__

inline const std::string TAG_NAME_URL = "url";

// TESTME: url_node
// TODO: docs for 'url_node'
struct url_node : public internal::node
{
	std::shared_ptr<urls::IPattern> pattern;
	std::vector<std::shared_ptr<internal::FilterExpression>> params;
	std::string var_name;

	std::string render(abc::IContext* ctx) override;
};

// TESTME: make_url_tag
// TODO: docs for 'make_url_tag'
extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_url_tag(const std::vector<std::shared_ptr<urls::IPattern>>& patterns);

__RENDER_TAGS_END__
