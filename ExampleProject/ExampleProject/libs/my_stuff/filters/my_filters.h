/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include <xalwart/core/object/object.h>
#include <xalwart/core/types/value.h>
#include <xalwart/render/library/base.h>

using ObjectPtr = std::shared_ptr<xw::core::object::Object>;
using String = xw::core::types::Value<std::string>;


extern ObjectPtr toUpper(const ObjectPtr& obj, const xw::render::lib::KwArgs&);

extern ObjectPtr rmDigits(const ObjectPtr& obj, const xw::render::lib::KwArgs& kwargs);

extern ObjectPtr getType(const ObjectPtr& obj, const xw::render::lib::KwArgs&);
