/**
 * management/commands/runserver.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Command to run the server from command line.
 */

#pragma once

// Base libraries.
#include <xalwart.base/kwargs.h>
#include <xalwart.base/net/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/command.h"


__MANAGEMENT_COMMANDS_BEGIN__

// TESTME: StartServerCommand
// TODO: docs for 'StartServerCommand'
class StartServerCommand final : public xw::cmd::Command
{
private:
	const std::string DEFAULT_IPV4_HOST = "127.0.0.1";
	const std::string DEFAULT_IPV6_HOST = "[::1]";
	const uint16_t DEFAULT_PORT = 8000;
	const size_t DEFAULT_THREADS = 16;
	const size_t DEFAULT_RETRIES_COUNT = 5;

	std::string _host;
	uint16_t _port = DEFAULT_PORT;
	bool _use_ipv6 = false;
	size_t _threads_count = DEFAULT_THREADS;
	size_t _retries_count = DEFAULT_RETRIES_COUNT;

	std::shared_ptr<core::flags::StringFlag> _addr_port_flag;
	std::shared_ptr<core::flags::StringFlag> _addr_flag;
	std::shared_ptr<core::flags::Uint16Flag> _port_flag;
	std::shared_ptr<core::flags::UnsignedLongFlag> _threads_flag;
	std::shared_ptr<core::flags::BoolFlag> _use_ipv6_flag;
	std::shared_ptr<core::flags::BoolFlag> _no_colors_flag;
	std::shared_ptr<core::flags::UnsignedLongFlag> _retries_count_flag;
	std::shared_ptr<core::flags::BoolFlag> _print_help_flag;

	re::Regex _ipv4_ipv6_port_regex;
	re::Regex _ipv4_regex;
	re::Regex _ipv6_regex;

	std::function<std::unique_ptr<net::abc::IServer>(
		log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
	)> make_server;

protected:
	void add_flags() override;
	void handle() override;
	void retrieve_args();

public:
	explicit StartServerCommand(
		conf::IModuleConfig* config,
		conf::Settings* settings,
		std::function<std::unique_ptr<net::abc::IServer>(
			log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
		)> make_server
	);

	// Returns command flags.
	[[nodiscard]]
	Kwargs get_kwargs() const override;
};

__MANAGEMENT_COMMANDS_END__
