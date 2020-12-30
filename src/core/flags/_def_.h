/**
 * core/flags/_def_.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: core/flags module's definitions.
 */

#pragma once

#include "../_def_.h"


// core::flags
#define __FLAGS_BEGIN__ __CORE_BEGIN__ namespace flags {
#define __FLAGS_END__ } __CORE_END__

// core::flags::internal
#define __FLAGS_INTERNAL_BEGIN__ __FLAGS_BEGIN__ namespace internal {
#define __FLAGS_INTERNAL_END__ } __FLAGS_END__
