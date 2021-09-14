/**
 * conf/loaders/yaml/limits.h
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

// TESTME: YAMLLimitsComponent
// TODO: docs for 'YAMLLimitsComponent'
class YAMLLimitsComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLLimitsComponent(Limits& limits);
};

__CONF_END__
