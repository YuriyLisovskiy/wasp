/**
 * core/management/app.h
 *
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
 *
 * Purpose: core/management app config.
 */

#pragma once

// Core libraries.
#include <xalwart.core/net/server.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../apps/module.h"


__MANAGEMENT_BEGIN__

class CoreManagementModuleConfig final : public apps::ModuleConfig
{
private:
	std::function<std::shared_ptr<net::IServer>(
		core::ILogger*,
		collections::Dict<std::string, std::string>
	)> _make_server;

public:
	explicit CoreManagementModuleConfig(
		conf::Settings* settings,
		std::function<std::shared_ptr<net::IServer>(
			core::ILogger*,
			collections::Dict<std::string, std::string>
		)> make_server
	);
	void commands() override;
};

__MANAGEMENT_END__
