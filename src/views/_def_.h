/**
 * views/_def_.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: views module's definitions.
 */

#pragma once

#include "../_def_.h"


// xw::views
#define __VIEWS_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace views {
#define __VIEWS_END__ } __MAIN_NAMESPACE_END__

// xw::views::internal
#define __VIEWS_INTERNAL_BEGIN__ __VIEWS_BEGIN__ namespace internal {
#define __VIEWS_INTERNAL_END__ } __VIEWS_END__
