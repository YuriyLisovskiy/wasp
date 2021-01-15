/**
 * management/commands/runserver.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: runs server from command line.
 */

#pragma once

// Core libraries.
#include <xalwart.core/net/server.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/app_command.h"


__MANAGEMENT_COMMANDS_BEGIN__

class StartServerCommand final : public xw::cmd::AppCommand
{
private:
	std::string _host;
	uint16_t _port = DEFAULT_PORT;
	bool _use_ipv6 = false;
	size_t _threads_count = DEFAULT_THREADS;

	std::shared_ptr<core::flags::StringFlag> _addr_port_flag;
	std::shared_ptr<core::flags::StringFlag> _addr_flag;
	std::shared_ptr<core::flags::Uint16Flag> _port_flag;
	std::shared_ptr<core::flags::UnsignedLongFlag> _threads_flag;
	std::shared_ptr<core::flags::BoolFlag> _use_ipv6_flag;
	std::shared_ptr<core::flags::BoolFlag> _log_color_flag;

	rgx::Regex _ipv4_ipv6_port_regex;
	rgx::Regex _ipv4_regex;
	rgx::Regex _ipv6_regex;

	const std::string DEFAULT_IPV4_HOST = "127.0.0.1";
	const std::string DEFAULT_IPV6_HOST = "[::1]";
	const uint16_t DEFAULT_PORT = 8000;
	const size_t DEFAULT_THREADS = 16;

	std::function<std::shared_ptr<net::IServer>(
		core::ILogger*,
		collections::Dict<std::string, std::string>
	)> make_server;

protected:
	void add_flags() final;
	void handle() final;
	void retrieve_args(
		std::string& host, uint16_t& port, bool& use_ipv6, size_t& threads_count
	);

public:
	explicit StartServerCommand(
		apps::IAppConfig* config,
		conf::Settings* settings,
		std::function<std::shared_ptr<net::IServer>(
			core::ILogger*,
			collections::Dict<std::string, std::string>
		)> make_server
	);
	collections::Dict<std::string, std::string> get_kwargs() override;
};

__MANAGEMENT_COMMANDS_END__
