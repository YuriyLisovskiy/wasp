/**
 * render/library/default.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Framework libraries.
#include "./library.h"
#include "../../conf/settings.h"


__RENDER_BEGIN__

class DefaultLibrary final : public Library
{
public:
	static const std::string FULL_NAME;

public:
	explicit DefaultLibrary(conf::Settings* settings);

	std::shared_ptr<collections::Dict<std::string, filter_t>> get_filters() override;
	std::shared_ptr<collections::Dict<std::string, tag_t>> get_tags() override;
	std::string name() override;
};

__RENDER_END__
