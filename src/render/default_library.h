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
	static inline constexpr const char* FULL_NAME = "xw::render::DefaultLibrary";

public:
	inline explicit DefaultLibrary(conf::Settings* settings) : Library(settings)
	{
	}

	[[nodiscard]]
	inline std::map<std::string, Filter> get_filters() const override
	{
		return {};
	}

	[[nodiscard]]
	std::map<std::string, Tag> get_tags() const override;

	[[nodiscard]]
	inline std::string name() const override
	{
		return DefaultLibrary::FULL_NAME;
	}
};

__RENDER_END__
