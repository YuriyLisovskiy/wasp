/**
 * views/args.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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
private:
	std::map<std::string, std::string> _map;

public:
	Args() = default;
	explicit Args(const std::map<std::string, std::string>& src);

	int get_int(const std::string& key, int _default = 0);
	std::string get_str(const std::string& key, const std::string& _default = "");

	bool contains(const std::string& key);

private:
	static bool is_number(const std::string& val);
};

__VIEWS_END__
