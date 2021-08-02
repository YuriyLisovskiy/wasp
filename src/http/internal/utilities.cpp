/**
 * http/internal/utilities.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./utilities.h"


__HTTP_INTERNAL_UTIL_BEGIN__

void trim_ascii_space(std::string& s)
{
	while (!s.empty() && is_ascii_space(s.front()))
	{
		s = s.substr(1);
	}

	while (!s.empty() && is_ascii_space(s.back()))
	{
		s.pop_back();
	}
}

__HTTP_INTERNAL_UTIL_END__
