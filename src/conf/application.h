/**
 * conf/application.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An implementation of main application.
 */

#pragma once

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./settings.h"
#include "../middleware/types.h"


__CONF_BEGIN__

class Application
{
public:
	explicit Application(const std::string& version, conf::Settings* settings);

	void execute(int argc, char** argv) const;

	[[nodiscard]]
	inline Version get_version() const
	{
		return this->version;
	}

protected:
	using HandlerFunction = std::function<net::StatusCode(
		net::RequestContext*, const std::map<std::string, std::string>& /* environment */
	)>;

	Version version;
	conf::Settings* settings;

	[[nodiscard]]
	std::unique_ptr<http::abc::IHttpResponse> error_response(
		http::Request* request, net::StatusCode status_code, const std::string& message
	) const;

	[[nodiscard]]
	middleware::Function build_controller_handler() const;

	[[nodiscard]]
	middleware::Function build_middleware_chain() const;

	[[nodiscard]]
	HandlerFunction build_server_handler() const;

	[[nodiscard]]
	bool static_is_allowed(const std::string& static_url) const;

	void build_static_pattern(
		std::vector<std::shared_ptr<urls::IPattern>>& patterns,
		const std::string& root, const std::string& url, const std::string& name
	) const;

	void build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const;

	[[nodiscard]]
	virtual std::shared_ptr<http::Request> build_request(
		net::RequestContext* context, std::map<std::string, std::string> env
	) const;

	[[nodiscard]]
	uint start_response(net::RequestContext* ctx, const std::unique_ptr<http::abc::IHttpResponse>& response) const;

	net::StatusCode finish_response(net::RequestContext* ctx, http::abc::IHttpResponse* response) const;

private:
	std::string _help_message;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> _commands;

	void _setup_commands();

	void _extend_settings_commands(
		const std::vector<std::shared_ptr<cmd::BaseCommand>>& from, const std::string& module_name
	);
};

__CONF_END__
