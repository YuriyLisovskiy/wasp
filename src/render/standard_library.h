/**
 * render/standard_library.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./library.h"


__RENDER_BEGIN__

// TESTME: DefaultLibrary
// TODO: docs for 'DefaultLibrary'
class StandardLibrary final : public Library
{
public:
	static inline constexpr const char* NAME = "xw::render::StandardLibrary";

	inline explicit StandardLibrary(conf::Settings* settings) : Library(settings)
	{
	}

	[[nodiscard]]
	inline std::map<std::string, Filter> get_filters() const override
	{
		return {};
	}

	[[nodiscard]]
	std::map<std::string, render::abc::ILibrary::Function> get_functions() const override;

	[[nodiscard]]
	inline std::string name() const override
	{
		return StandardLibrary::NAME;
	}
};

__RENDER_END__
