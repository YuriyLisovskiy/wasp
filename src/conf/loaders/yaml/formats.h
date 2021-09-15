/**
 * settings/components/yaml/formats.h
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

// TESTME: YAMLFormatsComponent
// TODO: docs for 'YAMLFormatsComponent'
class YAMLFormatsComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLFormatsComponent(Formats& formats);
};

__CONF_END__
