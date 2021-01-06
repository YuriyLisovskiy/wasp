/**
 * http/_def_.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: http module's definitions.
 */

#pragma once

#include "../_def_.h"


// http
#define __HTTP_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace http {
#define __HTTP_END__ } __MAIN_NAMESPACE_END__

// http::internal
#define __HTTP_INTERNAL_BEGIN__ __HTTP_BEGIN__ namespace internal {
#define __HTTP_INTERNAL_END__ } __HTTP_END__

// http::meta
#define __META_BEGIN__ __HTTP_BEGIN__ namespace meta {
#define __META_END__ } __HTTP_END__
