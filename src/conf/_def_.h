/**
 * conf/_def_.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: conf module's definitions.
 */

#pragma once

#include "../_def_.h"


// conf
#define __CONF_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace conf {
#define __CONF_END__ } __MAIN_NAMESPACE_END__

// conf::internal
#define __CONF_INTERNAL_BEGIN__ __CONF_BEGIN__ namespace internal {
#define __CONF_INTERNAL_END__ } __CONF_END__
