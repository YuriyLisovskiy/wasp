/**
 * core/_def_.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: core module's definitions.
 */

#pragma once

#include "../_def_.h"


// core::mime
#define __MIME_BEGIN__ __CORE_BEGIN__ namespace mime {
#define __MIME_END__ } __CORE_END__

// core::encoding
#define __ENCODING_BEGIN__ __CORE_BEGIN__ namespace encoding {
#define __ENCODING_END__ } __CORE_END__

// core::encoding::internal
#define __ENCODING_INTERNAL_BEGIN__ __CORE_BEGIN__ namespace internal {
#define __ENCODING_INTERNAL_END__ } __CORE_END__

// xw::parsers
#define __PARSERS_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace parsers {
#define __PARSERS_END__ } __MAIN_NAMESPACE_END__

// xw::files
#define __FILES_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace files {
#define __FILES_END__ } __MAIN_NAMESPACE_END__
