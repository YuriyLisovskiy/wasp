/*
 * Copyright (c) 2020 Yura Lisovskiy
 */

#include "./my_filters.h"

#include <xalwart/core/strings.h>


ObjectPtr toUpper(const ObjectPtr& obj, const xw::render::lib::KwArgs&)
{
	auto val = obj->__str__();
	for (char& ch : val)
	{
		ch = (char) std::toupper(ch);
	}

	return std::make_shared<String>(val);
}

ObjectPtr rmDigits(const ObjectPtr& obj, const xw::render::lib::KwArgs& kwargs)
{
	auto val = obj->__str__();
	auto digit = kwargs.get("digit")->__str__();
	for (size_t i = 0; i < val.size(); i++)
	{
		if (val[i] == digit[0])
		{
			val.erase(i, 1);
			i--;
		}
	}

	return std::make_shared<String>(val);
}

ObjectPtr getType(const ObjectPtr& obj, const xw::render::lib::KwArgs&)
{
	auto obj_type = obj->__type__();
	auto name = xw::core::str::replace(
		xw::core::str::replace(obj_type.name(), "<", "&lt"),
		">",
		"&gt"
	);
	return std::make_shared<String>(
		obj_type.namespace_() + "::" + name
	);
}
