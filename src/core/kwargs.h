/**
 * core/kwargs.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Contains keyword-arguments based on 'std::map'.
 * It is used for virtual functions with variable arguments
 * count.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>
#include <iostream>

// Core libraries.
#include <xalwart.base/utility.h>

// Module definitions.
#include "./_def_.h"


__CORE_BEGIN__

class Kwargs final
{
private:
	std::map<std::string, std::string> _map;

public:
	Kwargs() = default;

	inline explicit Kwargs(std::map<std::string, std::string> src) : _map(std::move(src))
	{
	}

	template <typename T>
	inline T get(const std::string& key, T _default={}) const
	{
		if (this->contains(key))
		{
			try
			{
				return util::as<T>((const char*)this->_map.at(key).c_str());
			}
			catch (const std::invalid_argument&)
			{
			}
			catch (const std::range_error&)
			{
			}
		}

		return _default;
	}

	[[nodiscard]]
	inline bool contains(const std::string& key) const
	{
		return this->_map.find(key) != this->_map.end();
	}
};

__CORE_END__
