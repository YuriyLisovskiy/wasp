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
#include <memory>
#include <string>

// Base libraries.
#include <xalwart.base/net/request_context.h>

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
	using HandlerFunction = std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)>;

	HandlerFunction _handler_function;

public:
	explicit CoreModuleConfig(conf::Settings* settings, HandlerFunction handler) :
		ModuleConfig(settings), _handler_function(std::move(handler))
	{
	}

	void commands() override;
};

__MANAGEMENT_END__
