/**
 * utility/crypto/utility.h
 *
 * Copyright (c) 2020-2021 Yuriy Lisovskiy
 *
 * HMAC hashing implementation.
 */

#pragma once

// C++ libraries.
#include <memory>

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "./abc.h"
#include "./md5.h"


__CRYPTO_BEGIN__

// TESTME: _get_default_hash_function
// TODO: docs for '_get_default_hash_function'
inline std::unique_ptr<abc::IHash> _get_default_hash_function()
{
	return std::make_unique<MD5>();
}

__CRYPTO_END__
