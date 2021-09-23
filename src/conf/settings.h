/**
 * conf/settings.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Entire application's settings.
 */

#pragma once

// C++ libraries.
#include <optional>

// Base libraries.
#include <xalwart.base/kwargs.h>
#include <xalwart.base/datetime.h>
#include <xalwart.base/string_utils.h>
#include <xalwart.base/utility.h>
#include <xalwart.base/path.h>
#include <xalwart.base/yaml/yaml-cpp/yaml.h>
#include <xalwart.base/net/abc.h>
#include <xalwart.base/net/status.h>
#include <xalwart.base/abc/base.h>
#include <xalwart.base/abc/render.h>

// ORM libraries.
#include <xalwart.orm/client.h>
#include <xalwart.orm/db/migration.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "./types.h"
#include "../middleware/types.h"


__CONF_BEGIN__

class Settings
{
public:
	// For development purposes only.
	bool DEBUG = false;

	// By default, it should be current working directory.
	std::string BASE_DIR;

	std::shared_ptr<abc::Logger> LOGGER = nullptr;

	bool THROW_ON_INVALID_REQUEST_CONTENT_TYPE = true;

	// A secret key for this particular installation. Used in secret-key
	// hashing algorithms. Set this in your settings.
	std::string SECRET_KEY;

	// Hosts/domain names that are valid for this site.
	//
	// For example: "127.0.0.1" - matches localhost, "*" - matches all
	std::vector<std::string> ALLOWED_HOSTS;

	// Local time zone for this installation. All choices can be found here:
	// https://en.wikipedia.org/wiki/List_of_tz_zones_by_name (although not all
	// systems may support all possibilities). When USE_TZ is true, this is
	// interpreted as the default user time zone.
	std::shared_ptr<dt::Timezone> TIMEZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);

	// If you set this to True, Django will use timezone-aware datetimes.
	bool USE_TIMEZONE = false;

	// Default charset to use for all HttpResponse objects, if a MIME type isn't
	// manually specified. It's used to construct the Content-Type header.
	std::string CHARSET = "utf-8";

	// Vector of patterns which will be loaded from the first
	// module from 'MODULES'.
	std::vector<std::shared_ptr<urls::IPattern>> URLPATTERNS;

	// List of ModuleConfig-derived objects representing modules.
	// Order is required. The first item is interpreted as main
	// module configuration.
	std::vector<std::shared_ptr<IModuleConfig>> MODULES;

	// Engine for rendering templates.
	std::shared_ptr<abc::render::IEngine> TEMPLATE_ENGINE = nullptr;

	// Default database instance.
	std::shared_ptr<orm::Client> DB = nullptr;

	// List of databases.
	std::map<std::string, std::shared_ptr<orm::Client>> DATABASES;

	// Whether to append trailing slashes to URLs.
	bool APPEND_SLASH = true;

	// List of regular expression objects representing User-Agent strings
	// that are not allowed to visit any page, systemwide. Use this for bad
	// robots/crawlers.
	//
	// Here are a few examples:
	//   DISALLOWED_USER_AGENTS = {
	//       re::Regex(R"(NaverBot.*)"),
	//       re::Regex(R"(EmailSiphon.*)"),
	//       re::Regex(R"(SiteSucker.*)"),
	//       re::Regex(R"(sohu-search.*)")
	//   };
	std::vector<re::Regex> DISALLOWED_USER_AGENTS;

	// List of compiled regular expression objects representing URLs that need not
	// be reported by BrokenLinkEmailsMiddleware.
	//
	// Here are a few examples:
	//   IGNORABLE_404_URLS = {
	//       rgx::Regex(R"(/apple-touch-icon.*\.png)"),
	//       rgx::Regex(R"(/favicon.ico)"),
	//       rgx::Regex(R"(/robots.txt)"),
	//       rgx::Regex(R"(/phpmyadmin/)"),
	//       rgx::Regex(R"(/apple-touch-icon.*\.png)")
	//   };
	std::vector<re::Regex> IGNORABLE_404_URLS;

	// A pair of absolute filesystem path (root) to the directory that will hold user-uploaded files
	// and URL that handles the media served from root.
	//
	// Root example: "/var/www/example.com/media/".
	// URL examples: "http://example.com/media/", "http://media.example.com/".
	Static MEDIA;

	// A pair of absolute path (root) to the directory static files should be collected to
	// and URL that handles the static files served from root.
	//
	// Root example: "/var/www/example.com/static/".
	// URL example: "http://example.com/static/", "http://static.example.com/".
	Static STATIC;

	Limits LIMITS = {
		// Maximum size, in bytes, of a request before it will be streamed to the
		// file system instead of into memory.
		.FILE_UPLOAD_MAX_MEMORY_SIZE = 2621440,

		// Maximum size in bytes of request data (excluding file uploads) that will be
		// read before a SuspiciousOperation (RequestDataTooBig) is raised.
		.DATA_UPLOAD_MAX_MEMORY_SIZE = 2621440,

		// Maximum number of GET/POST parameters that will be read before a
		// SuspiciousOperation (TooManyFieldsSent) is thrown.
		.DATA_UPLOAD_MAX_NUMBER_FIELDS = 1000,

		// Maximum header length that will be read before returning HTTP 431 error code.
		.MAX_HEADER_LENGTH = 65535,

		// Maximum number of headers per request.
		.MAX_HEADERS_COUNT = 100
	};

	// Whether to prepend the "www." subdomain to URLs that don't have it.
	bool PREPEND_WWW = false;

	Formats FORMATS = {
		// Default formatting for date objects.
		.DATE_FORMAT = "%b %d, %Y",

		// Default formatting for datetime objects.
		.DATETIME_FORMAT = "%b %d, %Y, %T",

		// Default formatting for time objects.
		.TIME_FORMAT = "%T",

		// Default formatting for date objects when only the year and month are relevant.
		.YEAR_MONTH_FORMAT = "%B %Y",

		// Default formatting for date objects when only the month and day are relevant.
		.MONTH_DAY_FORMAT = "%B %d",

		// Default short formatting for date objects.
		.SHORT_DATE_FORMAT = "%m/%d/%Y",

		// Default short formatting for datetime objects.
		.SHORT_DATETIME_FORMAT = "%m/%d/%Y %T"
	};

	// First day of week, to be used on calendars
	// 0 means Sunday, 1 means Monday...
	uint FIRST_DAY_OF_WEEK = 0;

	// Decimal separator symbol
	char DECIMAL_SEPARATOR = '.';

	// Boolean that sets whether to add thousand separator when formatting numbers
	bool USE_THOUSAND_SEPARATOR = false;

	// A thousand separator symbol
	char THOUSAND_SEPARATOR = ',';

	// Default X-Frame-Options header value
	XFrameOptions X_FRAME_OPTIONS = XFrameOptions::SameOrigin;

	bool USE_X_FORWARDED_HOST = false;
	bool USE_X_FORWARDED_PORT = false;

	// List of middleware to use. Order is important; in the request phase, this
	// middleware will be applied in the order given, and in the response
	// phase the middleware will be applied in reverse order.
	std::vector<middleware::Handler> MIDDLEWARE;

	// Settings for CSRF cookie.
	CrossSiteRequestForgery CSRF = {
		.COOKIE = {
			.NAME = "csrftoken",
			.AGE = 60 * 60 * 24 * 7 * 52,
			.DOMAIN = "",
			.PATH = "/",
			.SECURE = false,
			.HTTP_ONLY = false,
			.SAME_SITE = "Lax"
		},
		.HEADER_NAME = "X-XSRF-TOKEN",
		.TRUSTED_ORIGINS = {},
		.USE_SESSIONS = false
	};

	// SSL settings (will be added in future).
	bool USE_SSL = false;

	// Used in `Security` middleware.
	Secure SECURE = {
		.BROWSER_XSS_FILTER = false,
		.CONTENT_TYPE_NO_SNIFF = true,
		.HSTS_INCLUDE_SUBDOMAINS = false,
		.HSTS_PRELOAD = false,
		.HSTS_SECONDS = 0,
		.REDIRECT_EXEMPT = {},
		.REFERRER_POLICY = "",
		.SSL_HOST = "",
		.SSL_REDIRECT = false,

		// If your module is behind a proxy that sets a header to specify secure
		// connections, AND that proxy ensures that user-submitted headers with the
		// same name are ignored (so that people can't spoof it), set this value to
		// a `std::pair` of (header_name, header_value). For any requests that come in with
		// that header/value, request.is_secure() will return true.
		// WARNING! Only set this if you fully understand what you're doing. Otherwise,
		// you may be opening yourself up to a security risk.
		.PROXY_SSL_HEADER = Secure::Header{
			.name = "X-Forwarded-Proto",
			.value = "https"
		}
	};

	explicit Settings(const std::string& base_dir);

	virtual ~Settings() = default;

	[[nodiscard]]
	virtual std::string render_html_error_template(const net::Status& status, const std::string& message) const;

	[[nodiscard]]
	virtual std::string render_json_error_template(const net::Status& status, const std::string& message) const;

	virtual void prepare();

	virtual void check();

	virtual inline void register_components()
	{
		this->register_template_loaders();
		this->register_template_libraries();
		this->register_middleware();
		this->register_migrations();
		this->register_modules();
	}

	virtual inline void register_modules()
	{
	}

	virtual inline void register_middleware()
	{
	}

	virtual inline void register_template_libraries()
	{
	}

	virtual inline void register_template_loaders()
	{
	}

	virtual inline void register_migrations()
	{
	}

	virtual inline std::unique_ptr<net::abc::Server> build_server(
		const std::function<net::StatusCode(
			net::RequestContext*, const std::map<std::string, std::string>& /* environment */
		)>& handler,
		const Kwargs& kwargs
	)
	{
		return nullptr;
	}

	[[nodiscard]]
	std::shared_ptr<IModuleConfig> build_module(const std::string& full_name) const;

	[[nodiscard]]
	inline middleware::Handler get_middleware_by_name(const std::string& name) const
	{
		return this->_middleware.contains(name) ? this->_middleware.at(name) : nullptr;
	}

	[[nodiscard]]
	std::shared_ptr<abc::render::ILibrary> build_template_library(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<abc::render::ILoader> build_template_loader(const std::string& full_name) const;

	std::list<std::shared_ptr<orm::db::Migration>> build_migrations(orm::abc::ISQLDriver* driver);

protected:
	template <class T>
	[[nodiscard]]
	inline std::string get_name_or(const std::string& full_name) const
	{
		if (full_name.empty())
		{
			return demangle(typeid(T).name());
		}

		return full_name;
	}

	template <module_config_type ModuleConfigT>
	inline void module(const std::string& custom_name="")
	{
		auto name = this->get_name_or<ModuleConfigT>(custom_name);
		if (this->LOGGER && this->_modules.find(name) != this->_modules.end())
		{
			this->LOGGER->warning("module '" + name + "' will be overwritten");
		}

		this->_modules[name] = [this, name]() -> std::shared_ptr<IModuleConfig> {
			auto module = std::make_shared<ModuleConfigT>(this);
			module->init(name);
			return module;
		};
	}

	void middleware(const std::string& name, middleware::Handler handler);

	template <abc::render::library_type LibraryT>
	inline void library(const std::string& custom_name="")
	{
		auto name = this->get_name_or<LibraryT>(custom_name);
		if (this->LOGGER && this->_libraries.find(name) != this->_libraries.end())
		{
			this->LOGGER->warning("library '" + name + "' will be overwritten");
		}

		this->_libraries[name] = [this]() -> std::shared_ptr<abc::render::ILibrary> {
			return std::make_shared<LibraryT>(this);
		};
	}

	template <abc::render::loader_type LoaderT>
	inline void loader(const std::string& custom_name="")
	{
		auto name = this->get_name_or<LoaderT>(custom_name);
		if (this->LOGGER && this->_loaders.find(name) != this->_loaders.end())
		{
			this->LOGGER->warning("loader '" + name + "' will be overwritten");
		}

		this->_loaders[name] = [this]() -> std::shared_ptr<abc::render::ILoader> {
			return std::make_shared<LoaderT>(this);
		};
	}

	template <class MigrationT, class ...Args>
	inline void migration(Args&& ...args)
	{
		this->_migrations.push_back([args...](auto* driver) -> std::shared_ptr<orm::db::Migration>
		{
			return std::make_shared<MigrationT>(driver, std::forward<Args>(args)...);
		});
	}

private:
	std::map<std::string, middleware::Handler> _middleware;
	std::map<std::string, std::function<std::shared_ptr<abc::render::ILibrary>()>> _libraries;
	std::map<std::string, std::function<std::shared_ptr<IModuleConfig>()>> _modules;
	std::map<std::string, std::function<std::shared_ptr<abc::render::ILoader>()>> _loaders;
	std::list<std::function<std::shared_ptr<orm::db::Migration>(orm::abc::ISQLDriver* driver)>> _migrations;
};

__CONF_END__
