/**
 * utils/_def_.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Definitions of 'utils' module.
 */

#pragma once

#include "../_def_.h"

// xw::util
#define __UTIL_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace util {
#define __UTIL_END__ } __MAIN_NAMESPACE_END__

// xw::util::cache
#define __UTIL_CACHE_BEGIN__ __UTIL_BEGIN__ namespace cache {
#define __UTIL_CACHE_END__ } __UTIL_END__

// xw::util::cache::internal
#define __UTIL_CACHE_INTERNAL_BEGIN__ __UTIL_CACHE_BEGIN__ namespace internal {
#define __UTIL_CACHE_INTERNAL_END__ } __UTIL_CACHE_END__

// xw::util::fn
#define __UTIL_FN_BEGIN__ __UTIL_BEGIN__ namespace fn {
#define __UTIL_FN_END__ } __UTIL_END__
