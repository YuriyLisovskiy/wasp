/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <memory>

#include <xalwart/core/object/object.h>
#include <xalwart/core/types/value.h>
#include <xalwart/render/library/library.h>


using namespace xw::render;

class MyLib : public lib::Library
{
	using Object = std::shared_ptr<xw::core::object::Object>;
	using String = xw::core::types::Value<std::string>;

public:
	explicit MyLib(xw::conf::Settings* settings);
	std::shared_ptr<lib::Filters> get_filters() override;
	std::string name() override;
};
