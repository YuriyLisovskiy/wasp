/**
 * settings/components/yaml/secure.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// Base libraries.
#include <xalwart.base/config/components/yaml/default.h>

// Module definitions.
#include "../../_def_.h"

// Framework libraries.
#include "../../types.h"


__CONF_BEGIN__

// TESTME: YAMLSecureComponent
// TODO: docs for 'YAMLSecureComponent'
class YAMLSecureComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLSecureComponent(Secure& secure);
};

__CONF_END__
