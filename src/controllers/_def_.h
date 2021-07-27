/**
 * views/_def_.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: views module's definitions.
 */

#pragma once

#include "../_def_.h"

// xw::ctrl
#define __CONTROLLERS_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace ctrl {
#define __CONTROLLERS_END__ } __MAIN_NAMESPACE_END__

// xw::ctrl::internal
#define __CONTROLLERS_INTERNAL_BEGIN__ __CONTROLLERS_BEGIN__ namespace internal {
#define __CONTROLLERS_INTERNAL_END__ } __CONTROLLERS_END__
