/**
 * core/management/app.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: core/management app config.
 */

#pragma once

// Module definitions.
#include "../_def_.h"

// Framework libraries.
#include "../../apps/config.h"


__CORE_INTERNAL_BEGIN__

class CoreManagementAppConfig : public apps::AppConfig
{
public:
	explicit CoreManagementAppConfig(conf::Settings* settings);
	void commands() override;
};

__CORE_INTERNAL_END__
