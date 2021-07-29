/**
 * utils/functional.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * Utilities for functional-like programming.
 */

#pragma once

// C++ libraries.
#include <vector>
#include <functional>

// Module definitions.
#include "./_def_.h"


__UTIL_FN_BEGIN__

// TESTME: map
// TODO: docs for 'map'
template <typename InputT, typename ReturnT>
inline std::vector<ReturnT> map(
	const std::vector<InputT>& input, std::function<ReturnT(const InputT&)> apply
)
{
	std::vector<ReturnT> result;
	for (const auto& item : input)
	{
		result.push_back(apply(item));
	}

	return result;
}

__UTIL_FN_END__
