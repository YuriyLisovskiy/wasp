/**
 * management/commands/start_server.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Command to run the server from command line.
 */

#pragma once

// Base libraries.
#include <xalwart.base/net/request_context.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../../commands/command.h"
#include "../../commands/flags/default.h"


__MANAGEMENT_COMMANDS_BEGIN__

// TESTME: StartServerCommand
// TODO: docs for 'StartServerCommand'
class StartServerCommand : public xw::cmd::Command
{
public:
	explicit StartServerCommand(
		conf::IModuleConfig* config, conf::Settings* settings, std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)> handler
	);

	// Returns command flags.
	[[nodiscard]]
	Options get_options() const override;

protected:
	void add_flags() override;

	void handle() override;

	[[nodiscard]]
	virtual std::string get_startup_message(bool is_development_server) const;

private:
	const std::string DEFAULT_IPV4_HOST = "127.0.0.1";
	const std::string DEFAULT_IPV6_HOST = "[::1]";
	const uint16_t DEFAULT_PORT = 8000;
	const size_t DEFAULT_WORKERS_COUNT = 16;
	const size_t DEFAULT_TIMEOUT_SECONDS = 5;
	const size_t DEFAULT_TIMEOUT_MICROSECONDS = 0;
	const size_t DEFAULT_RETRIES_COUNT = 5;

	std::string _host;
	uint16_t _port = DEFAULT_PORT;
	bool _use_ipv6 = false;
	size_t _workers_count = DEFAULT_WORKERS_COUNT;
	size_t _timeout_seconds = DEFAULT_TIMEOUT_SECONDS;
	size_t _timeout_microseconds = DEFAULT_TIMEOUT_MICROSECONDS;
	size_t _retries_count = DEFAULT_RETRIES_COUNT;

	std::shared_ptr<xw::cmd::flags::StringFlag> _addr_port_flag;
	std::shared_ptr<xw::cmd::flags::StringFlag> _addr_flag;
	std::shared_ptr<xw::cmd::flags::Uint16Flag> _port_flag;
	std::shared_ptr<xw::cmd::flags::UnsignedLongFlag> _workers_flag;
	std::shared_ptr<xw::cmd::flags::UnsignedLongFlag> _timeout_seconds_flag;
	std::shared_ptr<xw::cmd::flags::UnsignedLongFlag> _timeout_microseconds_flag;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _use_ipv6_flag;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _no_colors_flag;
	std::shared_ptr<xw::cmd::flags::UnsignedLongFlag> _retries_count_flag;
	std::shared_ptr<xw::cmd::flags::BoolFlag> _print_help_flag;

	re::Regex _ipv4_ipv6_port_regex;
	re::Regex _ipv4_regex;
	re::Regex _ipv6_regex;

	std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)> _handler_function;

	void _parse_args();
};

__MANAGEMENT_COMMANDS_END__
