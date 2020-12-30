/**
 * core/management/app.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: core/management app config.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../apps/config.h"


__MANAGEMENT_BEGIN__

class CoreManagementAppConfig final : public apps::AppConfig
{
public:
	explicit CoreManagementAppConfig(conf::Settings* settings);
	void commands() override;
};

__MANAGEMENT_END__
