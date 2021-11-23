/**
 * conf/loaders/yaml/static.h
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 *
 * Purpose: TODO
 */

#pragma once

// C++ libraries.
#include <string>

// Base libraries.
#include <xalwart.base/config/components/yaml/default.h>

// Module definitions.
#include "../../_def_.h"

// Framework libraries.
#include "../../types.h"


__CONF_BEGIN__

// TESTME: YAMLStaticComponent
// TODO: docs for 'YAMLStaticComponent'
class YAMLStaticComponent : public config::YAMLMapComponent
{
public:
	explicit YAMLStaticComponent(Static& static_, const std::string& base_directory);
};

__CONF_END__
