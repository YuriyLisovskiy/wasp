/**
 * management/module.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Management module's config.
 */

#pragma once

// C++ libraries.
#include <memory>

// Base libraries.
#include <xalwart.base/net/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/module.h"


__MANAGEMENT_BEGIN__

using MakeServerFunction = std::function<std::unique_ptr<net::abc::IServer>(
	log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
)>;

// TESTME: CoreModuleConfig
// TODO: docs for 'CoreModuleConfig'
class CoreModuleConfig final : public conf::ModuleConfig
{
private:
	MakeServerFunction _make_server;

public:
	explicit CoreModuleConfig(conf::Settings* settings, MakeServerFunction make_server) :
		ModuleConfig(settings), _make_server(std::move(make_server))
	{
	}

	void commands() override;
};

__MANAGEMENT_END__
