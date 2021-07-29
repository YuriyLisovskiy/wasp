/**
 * http/status.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Hold all available http statuses, which contains status code,
 * 	reason phrase and brief explanation.
 */

#pragma once

// C++ libraries.
#include <string>

// Base modules.
#include <xalwart.base/collections/dictionary.h>

// Module definitions.
#include "./_def_.h"


__HTTP_INTERNAL_BEGIN__

extern collections::Dictionary<unsigned short int, std::pair<std::string, std::string>> HTTP_STATUS;

__HTTP_INTERNAL_END__
