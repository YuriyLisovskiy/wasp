/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <memory>

#include <xalwart/core/object/object.h>
#include <xalwart/core/types/value.h>
#include <xalwart/render/library/library.h>


using namespace xw::render;
using namespace xw::core;
using namespace xw::core::object;

class MyFirstLib : public lib::Library
{
public:
	explicit MyFirstLib(xw::conf::Settings* settings) : Library(settings)
	{
	}

	std::shared_ptr<lib::Filters> get_filters() override
	{
		return std::make_shared<lib::Filters>(std::map<std::string, lib::Filter>{
			{"to_upper", [](const std::shared_ptr<Object>& obj, const lib::KwArgs&) -> std::shared_ptr<Object>{
				auto val = obj->__str__();
				for (char& ch : val)
				{
					ch = (char) std::toupper(ch);
				}

				return std::make_shared<types::Value<std::string>>(val);
			}},
			{"rm_digits", [](const std::shared_ptr<Object>& obj, const lib::KwArgs& kwargs) -> std::shared_ptr<Object>{
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

				return std::make_shared<types::Value<std::string>>(val);
			}},
			{"type", [](const std::shared_ptr<Object>& obj, const lib::KwArgs&) -> std::shared_ptr<Object>{
				auto obj_type = obj->__type__();
				auto name = str::replace(
					str::replace(obj_type.name(), "<", "&lt"),
					">",
					"&gt"
				);
				return std::make_shared<types::Value<std::string>>(
					obj_type.namespace_() + "::" + name
				);
			}}
		});
	}

	std::shared_ptr<lib::Tags> get_tags() override
	{
		return nullptr;
	}

	std::string name() override
	{
		return "MyFirstLib";
	}
};
