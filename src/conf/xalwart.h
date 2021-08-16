/**
 * conf/xalwart.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * An implementation of main application.
 */

#pragma once

// Base libraries.
#include <xalwart.base/net/abc.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "../conf/settings.h"


__CONF_BEGIN__

class MainApplication
{
private:
	std::string _help_message;

	// List of commands to run from command line.
	std::map<std::string, std::shared_ptr<cmd::BaseCommand>> _commands;

private:
	void _setup_commands(net::HandlerFunc handler);

	void _extend_settings_commands(
		const std::vector<std::shared_ptr<cmd::BaseCommand>>& from, const std::string& module_name
	);

protected:
	Version version;

	conf::Settings* settings;

	std::function<std::unique_ptr<net::abc::IServer>(
		log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
	)> server_initializer;

protected:
	[[nodiscard]]
	net::HandlerFunc make_handler() const;

	[[nodiscard]]
	bool static_is_allowed(const std::string& static_url) const;

	void build_static_pattern(
		std::vector<std::shared_ptr<urls::IPattern>>& patterns,
		const std::string& root, const std::string& url, const std::string& name
	) const;

	void build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const;

	[[nodiscard]]
	http::Response::Result process_request(std::shared_ptr<http::Request>& request) const;

	[[nodiscard]]
	http::Response::Result process_urlpatterns(
		std::shared_ptr<http::Request>& request, std::vector<std::shared_ptr<urls::IPattern>>& urlpatterns
	) const;

	[[nodiscard]]
	http::Response::Result process_response(
		std::shared_ptr<http::Request>& request, std::shared_ptr<http::abc::IHttpResponse>& response
	) const;

	[[nodiscard]]
	std::shared_ptr<http::Request> build_request(
		net::RequestContext* ctx, collections::Dictionary<std::string, std::string> env
	) const;

	[[nodiscard]]
	uint start_response(net::RequestContext* ctx, const http::Response::Result& result) const;

	void finish_response(net::RequestContext* ctx, http::abc::IHttpResponse* response) const;

public:
	explicit MainApplication(
		const std::string& version,
		conf::Settings* settings,
		std::function<std::unique_ptr<net::abc::IServer>(
			log::ILogger*, const Kwargs&, std::shared_ptr<dt::Timezone>
		)> server_initializer
	);

	void execute(int argc, char** argv);
};

__CONF_END__
