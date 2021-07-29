/**
 * core/_def_.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Definitions of 'core' module.
 */

#pragma once

#include "../_def_.h"

// xw::core
#define __CORE_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace core {
#define __CORE_END__ } __MAIN_NAMESPACE_END__

// xw::core::mime
#define __CORE_MIME_BEGIN__ __CORE_BEGIN__ namespace mime {
#define __CORE_MIME_END__ } __CORE_END__

// xw::core::files
#define __CORE_FILES_BEGIN__ __MAIN_NAMESPACE_BEGIN__ namespace files {
#define __CORE_FILES_END__ } __MAIN_NAMESPACE_END__
