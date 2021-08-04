/**
 * management/module.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Management module's config.
 */

#pragma once

// Base libraries.
#include <xalwart.base/net/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/module.h"


__MANAGEMENT_BEGIN__

// TESTME: CoreModuleConfig
// TODO: docs for 'CoreModuleConfig'
class CoreModuleConfig final : public conf::ModuleConfig
{
private:
	std::function<std::shared_ptr<net::abc::IServer>(
		log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
	)> _make_server;

public:
	explicit CoreModuleConfig(
		conf::Settings* settings,
		std::function<std::shared_ptr<net::abc::IServer>(
			log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
		)> make_server
	);
	void commands() override;
};

__MANAGEMENT_END__
