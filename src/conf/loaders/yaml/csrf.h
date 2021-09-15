/**
 * settings/components/yaml/csrf.h
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

// TESTME: YAMLCSRFComponent
// TODO: docs for 'YAMLCSRFComponent'
class YAMLCSRFComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLCSRFComponent(CSRF& csrf);
};

__CONF_END__
