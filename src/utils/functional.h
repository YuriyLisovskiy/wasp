/**
 * utils/functional.h
 *
 * Copyright (c) 2020 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <vector>
#include <functional>

// Module definitions.
#include "./_def_.h"


__FUNCTIONAL_BEGIN__

template <typename _InputT, typename _ReturnT>
extern std::vector<_ReturnT> map(
	const std::vector<_InputT>& input,
	std::function<_ReturnT(const _InputT&)> apply
)
{
	std::vector<_ReturnT> result;
	for (const auto& item : input)
	{
		result.push_back(apply(item));
	}

	return result;
}

__FUNCTIONAL_END__
