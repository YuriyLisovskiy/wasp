/**
 * conf/application.cpp
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 */

#include "./application.h"

// C++ libraries.
#include <iostream>
#include <csignal>

// Base libraries.
#include <xalwart.base/string_utils.h>

// Framework libraries.
#include "../management/module.h"
#include "../urls/resolver.h"
#include "../urls/pattern.h"
#include "../middleware/exception.h"
#include "../controllers/static.h"


__CONF_BEGIN__

std::shared_ptr<urls::IPattern> _build_static_pattern(
	const conf::Settings* settings,
	const std::string& static_url, const std::string& static_root, const std::string& name
)
{
	if (static_url.empty())
	{
		throw ImproperlyConfigured("empty static url is not permitted", _ERROR_DETAILS_);
	}

	if (name.empty())
	{
		throw ImproperlyConfigured("empty static url name is not permitted", _ERROR_DETAILS_);
	}

	auto controller_function = [static_root](
		http::Request* request, const std::tuple<std::string>& args, const Settings* settings_pointer
	) -> std::unique_ptr<http::HttpResponse>
	{
		return std::apply([request, static_root, settings_pointer](const std::string& p) mutable -> auto
		{
			ctrl::StaticController controller(settings_pointer, static_root);
			return controller.dispatch(request, p);
		}, args);
	};

	return std::make_unique<urls::Pattern<std::string>>(
		str::rtrim(static_url, "/") + "/" + "<path>(.*)", controller_function, name
	);
}

void initialize_signal_handlers()
{
#if defined(_WIN32) || defined(_WIN64)
	// Termination
	signal(SIGINT, &_throw_interruption_exception);
	signal(SIGTERM, &_throw_interruption_exception);

	// Segmentation fault.
	signal(SIGSEGV, &_throw_null_pointer_exception);
#else
	// Termination.
	struct sigaction termination_handler{};
	termination_handler.sa_handler = _throw_interruption_exception;
	sigemptyset(&termination_handler.sa_mask);
	termination_handler.sa_flags = 0;
	sigaction(SIGINT, &termination_handler, nullptr);
	sigaction(SIGTERM, &termination_handler, nullptr);

	// Segmentation fault.
	struct sigaction segmentation_fault_handler{};
	segmentation_fault_handler.sa_handler = _throw_null_pointer_exception;
	sigemptyset(&segmentation_fault_handler.sa_mask);
	segmentation_fault_handler.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &segmentation_fault_handler, nullptr);
#endif
}

Application& Application::configure()
{
	this->configure_settings();
	this->build_static_patterns();

	// Retrieve main module patterns and append them to result.
	this->build_module_patterns(this->settings->URLPATTERNS);

	this->setup_template_engine();
	this->setup_middleware();
	this->setup_commands();

	this->is_configured = true;
	return *this;
}

void Application::execute(int argc, char** argv) const
{
	if (!this->is_configured)
	{
		throw RuntimeError(
			"application is not configured, call `Application::configure()` method before execution"
		);
	}

	if (argc > 1)
	{
		try
		{
			this->execute_command(argv[1], argc, argv);
		}
		catch (const BaseException& exc)
		{
			this->settings->LOGGER->error(exc.what(), exc.line(), exc.function(), exc.file());
		}
		catch (const std::exception& exc)
		{
			this->settings->LOGGER->error(exc.what());
		}
		catch (...)
		{
			this->settings->LOGGER->error("Unknown error", _ERROR_DETAILS_);
		}
	}
	else
	{
		std::cout << this->get_usage_message();
	}
}

void Application::execute_command(const std::string& command_name, int argc, char** argv) const
{
	if (this->commands.contains(command_name))
	{
		this->commands.at(command_name)->run_from_argv(argc, argv);
	}
	else
	{
		std::cout << "Command \"" << command_name << "\" is not found.\n\n";
	}
}

std::string Application::get_usage_message() const
{
	size_t max_command_length = 0;
	for (auto& command : this->commands)
	{
		auto current_length = command.second->name().size();
		if (current_length > max_command_length)
		{
			max_command_length = current_length;
		}
	}

	std::string usage_message;
	for (const auto& command : this->commands)
	{
		std::string indent(max_command_length - command.second->name().size(), ' ');
		usage_message += "  " + command.second->name() + indent + "  " + command.second->help_message() + '\n';
	}

	if (!usage_message.empty())
	{
		usage_message = "Usage:\n  application [command]\n\n"
		                "Available Commands:\n" + usage_message +
		                "\nUse \"application [command] --help\" for more information about a command.";
	}
	else
	{
		usage_message = "Application has not commands.\n\n";
	}

	return usage_message;
}

Application::ServerHandler Application::get_application_handler() const
{
	return [this](
		net::RequestContext* context, const std::map<std::string, std::string>& environment
	) -> net::StatusCode
	{
		auto request = this->build_request(context, environment);
		auto middleware_chain = this->build_middleware_chain();
		auto response = middleware_chain(request.get());
		return this->send_response(context, response);
	};
}

middleware::Function Application::get_controller_handler() const
{
	return [this](http::Request* request) -> std::unique_ptr<http::HttpResponse>
	{
		require_non_null(request, _ERROR_DETAILS_);
		auto apply = urls::resolve(request->url().path, this->settings->URLPATTERNS);
		if (apply)
		{
			return apply(request, this->settings);
		}

		return this->get_error_response(request, 404, "The requested resource was not found.");
	};
}

middleware::Function Application::build_middleware_chain() const
{
	middleware::Function chain = this->get_controller_handler();
	auto middleware_count = (long long)this->settings->MIDDLEWARE.size() - 1;
	for (long long i = middleware_count; i >= 0; i--)
	{
		auto next_middleware = this->settings->MIDDLEWARE[i];
		if (next_middleware)
		{
			chain = next_middleware(chain);
		}
	}

	return chain;
}

void Application::build_module_patterns(std::vector<std::shared_ptr<urls::IPattern>>& patterns) const
{
	if (!this->settings->MODULES.empty())
	{
		for (auto& module : this->settings->MODULES)
		{
			if (!module)
			{
				throw NullPointerException("'module' is nullptr", _ERROR_DETAILS_);
			}

			auto modules_patterns = module->get_urlpatterns();
			patterns.insert(patterns.end(), modules_patterns.begin(), modules_patterns.end());
		}
	}
}

std::shared_ptr<http::Request> Application::build_request(
	net::RequestContext* context, std::map<std::string, std::string> environment
) const
{
	require_non_null(context, "'context' is nullptr", _ERROR_DETAILS_);
	context->body->set_limit((ssize_t)context->content_size);
	return std::make_shared<http::Request>(
		*context,
		this->settings->LIMITS.FILE_UPLOAD_MAX_MEMORY_SIZE,
		this->settings->LIMITS.DATA_UPLOAD_MAX_NUMBER_FIELDS,
		settings->LIMITS.MAX_HEADER_LENGTH,
		settings->LIMITS.MAX_HEADERS_COUNT,
		settings->LIMITS.DATA_UPLOAD_MAX_MEMORY_SIZE,
		std::move(environment)
	);
}

void Application::build_static_patterns()
{
	// Check if static files can be served and create necessary urls.
	this->add_static_pattern(
		this->settings->URLPATTERNS, this->settings->STATIC.ROOT, this->settings->STATIC.URL, "static"
	);
	this->add_static_pattern(
		this->settings->URLPATTERNS, this->settings->MEDIA.ROOT, this->settings->MEDIA.URL, "media"
	);
}

void Application::add_static_pattern(
	std::vector<std::shared_ptr<urls::IPattern>>& patterns,
	const std::string& root, const std::string& url, const std::string& name
) const
{
	if (!root.empty() && this->_static_is_allowed(url))
	{
		patterns.push_back(_build_static_pattern(this->settings, url, root, name));
	}
}

void Application::setup_commands()
{
	auto core_module = mgmt::CoreModuleConfig(this->settings, std::move(this->get_application_handler()));
	this->_append_commands(core_module.get_commands(), core_module.get_name());
	for (auto& installed_module : this->settings->MODULES)
	{
		this->_append_commands(installed_module->get_commands(), installed_module->get_name());
	}
}

void Application::setup_middleware()
{
	this->settings->MIDDLEWARE.insert(this->settings->MIDDLEWARE.begin(), middleware::Exception(this->settings));
}

std::unique_ptr<http::HttpResponse> Application::get_error_response(
	http::Request* request, net::StatusCode status_code, const std::string& message
) const
{
	auto [status, status_is_found] = net::get_status_by_code(status_code);
	if (!status_is_found)
	{
		require_non_null(this->settings->LOGGER.get(), _ERROR_DETAILS_)->warning(
			"Unknown status code: " + std::to_string(status_code)
		);
	}

	bool is_json = require_non_null(request, _ERROR_DETAILS_)->is_json();
	std::string content = is_json ?
		this->settings->render_json_error_template(status, message) :
		this->settings->render_html_error_template(status, message);
	return std::make_unique<http::Response>(content, status_code, is_json ? http::mime::APPLICATION_JSON : "");
}

net::StatusCode Application::send_response(
	net::RequestContext* ctx, const std::unique_ptr<http::HttpResponse>& response
) const
{
	if (!response)
	{
		this->settings->LOGGER->warning("Response was not instantiated, returned 204.", _ERROR_DETAILS_);
	}

	http::Response no_content(204);
	return this->finish_response(ctx, response ? response.get() : &no_content);
}

net::StatusCode Application::finish_response(net::RequestContext* context, http::HttpResponse* response) const
{
	require_non_null(context, "'context' is nullptr", _ERROR_DETAILS_);
	if (!context->response_writer)
	{
		throw NullPointerException("Unable to send response, response writer is nullptr", _ERROR_DETAILS_);
	}

	require_non_null(response, "'response' is nullptr", _ERROR_DETAILS_);
	if (response->is_streaming())
	{
		this->finish_streaming_response(context, response);
	}
	else
	{
		auto data = response->serialize();
		if (!context->response_writer->write(data.c_str(), data.size()))
		{
			this->settings->LOGGER->trace("Unable to send response", _ERROR_DETAILS_);
		}
	}

	return response->get_status();
}

void Application::finish_streaming_response(net::RequestContext* context, http::HttpResponse* response) const
{
	auto* streaming_response = dynamic_cast<http::StreamingResponse*>(response);
	if (!streaming_response)
	{
		throw NullPointerException(
			"Unable to cast response to streaming response, "
			"check if the response you returned is derived from 'xw::http::StreamingResponse'.",
			_ERROR_DETAILS_
		);
	}

	std::string chunk;
	while (!(chunk = streaming_response->get_chunk()).empty())
	{
		if (!context->response_writer->write(chunk.c_str(), chunk.size()))
		{
			this->settings->LOGGER->trace("Unable to send chunk", _ERROR_DETAILS_);
		}
	}

	response->close();
}

void Application::_append_command(const std::shared_ptr<cmd::AbstractCommand>& command, const std::string& module_name)
{
	if (this->_has_command(command))
	{
		this->settings->LOGGER->warning(
			"Module with name '" + module_name + "' overrides commands with '" + command->name() + "' command"
		);

		this->commands[command->name()] = command;
	}
	else
	{
		this->commands.insert(std::make_pair(command->name(), command));
	}
}

__CONF_END__
