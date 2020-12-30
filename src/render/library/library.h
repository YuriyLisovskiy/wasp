/**
 * render/library/library.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Render libraries.
#include <xalwart.render/library/base.h>

// Framework libraries.
#include "../../conf/settings.h"


__LIB_BEGIN__

class Library : public ILibrary
{
protected:
	conf::Settings* settings;

public:
	explicit Library(const std::string& lib_name, conf::Settings* settings);
	std::shared_ptr<Filters> get_filters() override;
	std::shared_ptr<Tags> get_tags() override;
};

__LIB_END__
