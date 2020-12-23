/**
 * http/status.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose:
 * 	Hold all available http statuses, which contains status code,
 * 	reason phrase and brief explanation.
 */

#pragma once

#ifdef _MSC_VER
#include <string>
#endif

// Core modules.
#include <xalwart.core/collections/dict.h>

// Module definitions.
#include "./_def_.h"


__HTTP_INTERNAL_BEGIN__

extern collections::Dict<unsigned short int, std::pair<std::string, std::string>> HTTP_STATUS;

__HTTP_INTERNAL_END__
