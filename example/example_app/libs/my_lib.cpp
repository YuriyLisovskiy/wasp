/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#include "./my_lib.h"

#include <xalwart/core/strings.h>


MyLib::MyLib(xw::conf::Settings* settings) : lib::Library(settings)
{
}

std::shared_ptr<lib::Filters> MyLib::get_filters()
{
	return std::make_shared<lib::Filters>(std::map<std::string, lib::Filter>{
		{"to_upper", [](const Object& obj, const lib::KwArgs&) -> Object{
			auto val = obj->__str__();
			for (char& ch : val)
			{
				ch = (char) std::toupper(ch);
			}

			return std::make_shared<String>(val);
		}},
		{"rm_digits", [](const Object& obj, const lib::KwArgs& kwargs) -> Object{
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
		}},
		{"type", [](const Object& obj, const lib::KwArgs&) -> Object{
			auto obj_type = obj->__type__();
			auto name = xw::core::str::replace(
				xw::core::str::replace(obj_type.name(), "<", "&lt"),
				">",
				"&gt"
			);
			return std::make_shared<String>(
					obj_type.namespace_() + "::" + name
			);
		}}
	});
}

std::string MyLib::name()
{
	return "MyLib";
}
