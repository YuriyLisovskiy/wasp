/**
 * render/default.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Framework libraries.
#include "./library.h"


__RENDER_BEGIN__

// TESTME: DefaultLibrary
// TODO: docs for 'DefaultLibrary'
class DefaultLibrary final : public Library
{
public:
	inline static const std::string FULL_NAME = "xw::render::DefaultLibrary";

public:
	inline explicit DefaultLibrary(conf::Settings* settings) : Library(settings)
	{
	}

	inline std::shared_ptr<collections::Dictionary<std::string, filter_t>> get_filters() override
	{
		return nullptr;
	}

	std::shared_ptr<collections::Dictionary<std::string, tag_t>> get_tags() override;

	inline std::string name() override
	{
		return DefaultLibrary::FULL_NAME;
	}
};

__RENDER_END__
