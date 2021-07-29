/**
 * render/library/library.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Render libraries.
#include <xalwart.render/library/abc.h>

// Framework libraries.
#include "../../conf/settings.h"


__RENDER_BEGIN__

class Library : public abc::ILibrary
{
protected:
	conf::Settings* settings;

public:
	explicit Library(const std::string& lib_name, conf::Settings* settings);
	std::shared_ptr<collections::Dictionary<std::string, filter_t>> get_filters() override;
	std::shared_ptr<collections::Dictionary<std::string, tag_t>> get_tags() override;
};

__RENDER_END__
