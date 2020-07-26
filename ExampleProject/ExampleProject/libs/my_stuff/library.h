/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#pragma once

#include <xalwart/render/library/library.h>


class MyStuff : public xw::render::lib::Library
{
public:
	explicit MyStuff(xw::conf::Settings* settings);
	std::string name() override;
	std::shared_ptr<xw::render::lib::Filters> get_filters() override;
};
