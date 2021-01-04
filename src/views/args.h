/**
 * views/args.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: class that contains url arguments based on std::map.
 */

#pragma once

// C++ libraries.
#include <map>
#include <string>

// Module definitions.
#include "./_def_.h"


__VIEWS_BEGIN__

class Args
{
public:
	Args() = default;
	explicit Args(std::map<std::string, std::string> src);

	[[nodiscard]]
	int get_int(const std::string& key, int _default = 0) const;

	[[nodiscard]]
	std::string get_str(const std::string& key, const std::string& _default = "") const;

	[[nodiscard]]
	bool contains(const std::string& key) const;

private:
	std::map<std::string, std::string> _map;

private:
	static bool _is_number(const std::string& val);
};

__VIEWS_END__
