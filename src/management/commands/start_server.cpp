/**
 * management/commands/start_server.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./start_server.h"

// Base libraries.
#include <xalwart.base/string_utils.h>
#include <xalwart.base/datetime.h>


__MANAGEMENT_COMMANDS_BEGIN__

StartServerCommand::StartServerCommand(
	conf::IModuleConfig* config, conf::Settings* settings, std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)> handler
) : Command(config, settings, "start-server", "Starts a web application"),
	_handler_function(std::move(handler))
{
	std::string ipv4 = R"((\d{1,3}(?:\.\d{1,3}){3})|localhost)";
	this->_ipv4_regex = re::Regex(ipv4);

	std::string ipv6 = R"(\[([a-fA-F0-9:]+)\]|\[localhost\])";
	this->_ipv6_regex = re::Regex(ipv6);

	std::string fqdn = R"([a-zA-Z0-9-]+(?:\.[a-zA-Z0-9-]+)*)";

	std::string port = R"(\d+)";
	this->_ipv4_ipv6_port_regex = re::Regex(
		R"((?:(()" + ipv4 + R"()|()" + ipv6 + R"()|()" + fqdn + R"()):)?()" + port + R"())"
	);
}

Options StartServerCommand::get_options() const
{
	auto options = Command::get_options();
	options.set("workers", std::to_string(this->_workers_count));
	options.set("retries", std::to_string(this->_retries_count));
	options.set("timeout_seconds", std::to_string(this->_timeout_seconds));
	options.set("timeout_microseconds", std::to_string(this->_timeout_microseconds));
	return options;
}

void StartServerCommand::add_flags()
{
	xw::cmd::Command::add_flags();
	this->_addr_port_flag = this->flag_set->make_string(
		"b", "bind", "", "Server address and port formatted as host:port"
	);
	this->_addr_flag = this->flag_set->make_string(
		"n", "hostname", "", "Server address"
	);
	this->_port_flag = this->flag_set->make_uint16(
		"p", "port", this->DEFAULT_PORT, "Server port"
	);
	this->_workers_flag = this->flag_set->make_unsigned_long(
		"t", "workers", this->DEFAULT_WORKERS_COUNT, "Parallel workers count"
	);
	this->_timeout_seconds_flag = this->flag_set->make_unsigned_long(
		"s", "timeout-seconds", this->DEFAULT_TIMEOUT_SECONDS, "Seconds timeout"
	);
	this->_timeout_microseconds_flag = this->flag_set->make_unsigned_long(
		"m", "timeout-microseconds", this->DEFAULT_TIMEOUT_MICROSECONDS, "Microseconds timeout"
	);
	this->_use_ipv6_flag = this->flag_set->make_bool(
		"i", "use-ipv6", false, "Use IPv6 address or not (used in case when host is set to 'localhost')"
	);
	this->_retries_count_flag = this->flag_set->make_unsigned_long(
		"r", "retries", this->DEFAULT_RETRIES_COUNT, "Max retries count to bind socket"
	);
}

bool StartServerCommand::handle()
{
	if (xw::cmd::Command::handle())
	{
		return true;
	}

	if (!this->settings->DEBUG && this->settings->ALLOWED_HOSTS.empty())
	{
		throw CommandError(
			"you must set 'allowed_hosts' if 'debug' is false.",
			_ERROR_DETAILS_
		);
	}

	this->_parse_args();
	auto server = this->settings->build_server(this->_handler_function, this->get_options());
	try
	{
		if (!server)
		{
			this->settings->LOGGER->error("server is not initialized", _ERROR_DETAILS_);
			return true;
		}

		server->bind(this->_host, this->_port);
		server->listen(this->get_startup_message(server->is_development()));
	}
	catch (const InterruptException& exc)
	{
		// TODO: remove this catch statement when will not be used.
		this->settings->LOGGER->debug("Interrupted");
	}
	catch (const BaseException& exc)
	{
		this->settings->LOGGER->error(exc);
	}
	catch (const std::exception& exc)
	{
		this->settings->LOGGER->error(exc.what(), _ERROR_DETAILS_);
	}

	server->close();
	return true;
}

std::string StartServerCommand::get_startup_message(bool is_development_server) const
{
	return dt::Datetime::now(this->settings->TIMEZONE).strftime("%B %d, %Y - %T") + "\n" +
	       v::framework_name + " version " + v::version.to_string() + "\n" +
	       "Starting the " + (is_development_server ? "development " : "") + "server at " +
	       "http://" + this->_host + ":" + std::to_string(this->_port) + "/\n" +
	       "Quit the server with CONTROL-C.";
}

void StartServerCommand::_parse_args()
{
	// Setup address and port.
	auto host_port_str = this->_addr_port_flag->get();
	if (!host_port_str.empty())
	{
		if (!this->_ipv4_ipv6_port_regex.search(host_port_str))
		{
			throw CommandError(host_port_str + " is not valid host:port pair", _ERROR_DETAILS_);
		}

		auto groups = this->_ipv4_ipv6_port_regex.groups();
		auto address = (groups[1].empty() ? this->DEFAULT_IPV4_HOST : groups[1]);
		this->_use_ipv6 = this->_ipv6_regex.match(address);
		auto trimmed_addr = str::trim(address, "[]");
		if (trimmed_addr == "localhost")
		{
			address = this->_use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}

		this->_host = address;
		this->_port = groups[4].empty() ? this->DEFAULT_PORT : (uint16_t) std::stoi(groups[4]);
	}
	else
	{
		auto address = this->_addr_flag->get();
		if (address.empty())
		{
			this->_use_ipv6 = this->_use_ipv6_flag->get();
			address = this->_use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}
		else
		{
			if (this->_ipv6_regex.match(address))
			{
				this->_use_ipv6 = true;
			}
			else if (!this->_ipv4_regex.match(address))
			{
				throw CommandError(
					this->_addr_flag->get_raw() + " is invalid address",
					_ERROR_DETAILS_
				);
			}
		}

		auto trimmed_addr = str::trim(address, "[]");
		if (trimmed_addr == "localhost")
		{
			address = this->_use_ipv6 ? this->DEFAULT_IPV6_HOST : this->DEFAULT_IPV4_HOST;
		}

		this->_host = address;
		if (this->_port_flag->valid())
		{
			this->_port = this->_port_flag->get();
		}
		else
		{
			throw CommandError(
				this->_port_flag->get_raw() + " is invalid port",
				_ERROR_DETAILS_
			);
		}
	}

	if (!this->_workers_flag->valid())
	{
		throw CommandError(
			"workers count is not a valid positive integer: " + this->_workers_flag->get_raw(),
			_ERROR_DETAILS_
		);
	}

	this->_workers_count = this->_workers_flag->get();

	if (!this->_timeout_seconds_flag->valid())
	{
		throw CommandError(
			"timeout seconds is not a valid positive integer: " + this->_timeout_seconds_flag->get_raw(),
			_ERROR_DETAILS_
		);
	}

	this->_timeout_seconds = this->_timeout_seconds_flag->get();

	if (!this->_timeout_microseconds_flag->valid())
	{
		throw CommandError(
			"timeout microseconds is not a valid positive integer: " + this->_timeout_microseconds_flag->get_raw(),
			_ERROR_DETAILS_
		);
	}

	this->_timeout_microseconds = this->_timeout_microseconds_flag->get();

	if (!this->_retries_count_flag->valid())
	{
		throw CommandError(
			"retries count is not a valid positive integer: " + this->_retries_count_flag->get_raw(),
			_ERROR_DETAILS_
		);
	}

	this->_retries_count = this->_retries_count_flag->get();
}

__MANAGEMENT_COMMANDS_END__
