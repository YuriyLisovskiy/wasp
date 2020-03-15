/*
 * Copyright (c) 2020 Yuriy Lisovskiy
 */

#pragma once

#include <memory>

#include "../../../src/core/object/object.h"
#include "../../../src/core/types/value.h"
#include "../../../src/render/library/base.h"


using namespace wasp::render;
using namespace wasp::core;
using namespace wasp::core::object;

class MyFirstLib : public lib::Library
{
public:
	lib::Filters get_filters() override
	{
		return lib::Filters({
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
					}
				}

				return std::make_shared<types::Value<std::string>>(val);
			}}
		});
	}

	lib::Tags get_tags() override
	{
		return {};
	}

	std::string name() override
	{
		return this->__type__().name();
	}
};
