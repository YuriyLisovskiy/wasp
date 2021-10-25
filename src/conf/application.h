/**
 * conf/application.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An implementation of main application.
 */

#pragma once

// C++ libraries.
#include <algorithm>
#include <memory>
#include <string>

// Base libraries.
#include <xalwart.base/exceptions.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./settings.h"
#include "../middleware/types.h"
#include "../urls/interfaces.h"


__CONF_BEGIN__

inline void _throw_interruption_exception(int signal)
{
	throw InterruptException("Execution is interrupted with signal: " + std::to_string(signal));
}

inline void _throw_null_pointer_exception(int)
{
	throw NullPointerException("invalid storage access (segmentation fault)");
}

// TESTME: _build_static_pattern
// TODO: docs for '_build_static_pattern'
extern std::shared_ptr<urls::IPattern> _build_static_pattern(
	const conf::Settings* settings,
	const std::string& static_url, const std::string& static_root, const std::string& name=""
);

extern void initialize_signal_handlers();

// TESTME: Application
// TODO: docs for 'Application'
class Application
{
public:
	explicit inline Application(conf::Settings* settings) : settings(settings), is_configured(false)
	{
	}

	virtual Application& configure();

	void execute(int argc, char** argv) const;

protected:
	using ServerHandler = std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)>;

	conf::Settings* settings = nullptr;
	bool is_configured;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::AbstractCommand>> commands;

	virtual void execute_command(const std::string& command_name, int argc, char** argv) const;

	[[nodiscard]]
	virtual std::string get_usage_message() const;

	[[nodiscard]]
	virtual middleware::Function get_controller_handler() const;

	[[nodiscard]]
	virtual middleware::Function build_middleware_chain() const;

	[[nodiscard]]
	virtual ServerHandler get_application_handler() const;

	virtual void build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const;

	[[nodiscard]]
	virtual std::shared_ptr<http::Request> build_request(
		net::RequestContext* context, std::map<std::string, std::string> env
	) const;

	virtual void build_static_patterns();

	virtual void add_static_pattern(
		std::vector<std::shared_ptr<urls::IPattern>>& patterns,
		const std::string& root, const std::string& url, const std::string& name
	) const;

	virtual void setup_commands();

	virtual void setup_middleware();

	virtual inline void setup_template_engine()
	{
		if (this->settings->TEMPLATE_ENGINE)
		{
			this->settings->TEMPLATE_ENGINE->load_libraries();
		}
	}

	virtual inline void configure_settings()
	{
		require_non_null(this->settings, "settings is not instantiated", _ERROR_DETAILS_);
		this->settings->prepare();
		this->settings->check();
	}

	[[nodiscard]]
	virtual std::unique_ptr<http::HttpResponse> get_error_response(
		http::Request* request, net::StatusCode status_code, const std::string& message
	) const;

	[[nodiscard]]
	virtual uint send_response(net::RequestContext* ctx, const std::unique_ptr<http::HttpResponse>& response) const;

	virtual net::StatusCode finish_response(net::RequestContext* context, http::HttpResponse* response) const;

	virtual void finish_streaming_response(net::RequestContext* context, http::HttpResponse* response) const;

private:
	[[nodiscard]]
	bool _static_is_allowed(const std::string& static_url) const
	{
		auto url = http::parse_url(static_url);

		// Allow serving local static files if debug and
		// static url is local.
		return this->settings->DEBUG && url.hostname().empty();
	}

	inline void _append_commands(
		const std::vector<std::shared_ptr<cmd::AbstractCommand>>& from, const std::string& module_name
	)
	{
		std::for_each(from.begin(),  from.end(), [this, module_name](const auto& command) -> void {
			this->_append_command(command, module_name);
		});
	}

	void _append_command(const std::shared_ptr<cmd::AbstractCommand>& command, const std::string& module_name);

	[[nodiscard]]
	inline bool _has_command(const auto& target_command) const
	{
		return std::find_if(
			this->commands.begin(), this->commands.end(),
			[target_command](const auto& command) -> bool { return target_command->name() == command.first; }
		) != this->commands.end();
	}
};

__CONF_END__
