/**
 * conf/settings.h
 *
 * Copyright (c) 2019-2021 Yuriy Lisovskiy
 *
 * Purpose: entire application's settings.
 */

#pragma once

// Base libraries.
#include <xalwart.base/datetime.h>
#include <xalwart.base/logger.h>
#include <xalwart.base/result.h>
#include <xalwart.base/string_utils.h>
#include <xalwart.base/utility.h>
#include <xalwart.base/path.h>
#include <xalwart.base/yaml/yaml-cpp/yaml.h>
#include <xalwart.base/net/abc.h>

// Render libraries.
#include <xalwart.render/abc.h>
#include <xalwart.render/library/abc.h>

// ORM libraries.
#include <xalwart.orm/client.h>
#include <xalwart.orm/db/migration.h>
#include <xalwart.orm/sqlite3/driver.h>

// Module definitions.
#include "./_def_.h"

// Framework libraries.
#include "./abc.h"
#include "../middleware/abc.h"
#include "../urls/url.h"
#include "../core/kwargs.h"


__CONF_BEGIN__

struct Settings
{
public:
	bool DEBUG = false;

	std::shared_ptr<log::ILogger> LOGGER = nullptr;

	// Build paths inside the project like this: path::join(BASE_DIR, ...)
	std::string BASE_DIR;

	// Hosts/domain names that are valid for this site.
	//
	// For example: "127.0.0.1" - matches localhost, "*" - matches all
	std::vector<std::string> ALLOWED_HOSTS;

	// Local time zone for this installation. All choices can be found here:
	// https://en.wikipedia.org/wiki/List_of_tz_zones_by_name (although not all
	// systems may support all possibilities). When USE_TZ is true, this is
	// interpreted as the default user time zone.
	std::shared_ptr<dt::Timezone> TIME_ZONE = std::make_shared<dt::Timezone>(dt::Timezone::UTC);

	// If you set this to True, Django will use timezone-aware datetimes.
	bool USE_TZ = false;

	// Default charset to use for all HttpResponse objects, if a MIME type isn't
	// manually specified. It's used to construct the Content-Type header.
	std::string CHARSET = "utf-8";

	// Root application where framework will load urlpatterns.
	//
	// ROOT_MODULE is the first installed module by default, it can
	// be overridden in project settings.
	std::shared_ptr<IModuleConfig> ROOT_MODULE = nullptr;

	// Vector of patterns which will be loaded from ROOT_MODULE.
	// To change this setting, setup ROOT_MODULE in your project
	// settings.
	std::vector<std::shared_ptr<urls::UrlPattern>> ROOT_URLCONF;

	// List of ModuleConfig-derived objects representing modules.
	// Order is required. The first item is interpreted as main
	// module configuration.
	std::vector<std::shared_ptr<IModuleConfig>> MODULES;

	// Engine for rendering templates.
	std::shared_ptr<render::abc::IEngine> TEMPLATE_ENGINE = nullptr;

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
	//     DISALLOWED_USER_AGENTS = {
	//         rgx::Regex(R"(NaverBot.*)"),
	//         rgx::Regex(R"(EmailSiphon.*)"),
	//         rgx::Regex(R"(SiteSucker.*)"),
	//         rgx::Regex(R"(sohu-search.*)")
	//     };
	std::vector<re::Regex> DISALLOWED_USER_AGENTS;

	// List of compiled regular expression objects representing URLs that need not
	// be reported by BrokenLinkEmailsMiddleware.
	//
	// Here are a few examples:
	//    IGNORABLE_404_URLS = {
	//        rgx::Regex(R"(/apple-touch-icon.*\.png)"),
	//        rgx::Regex(R"(/favicon.ico)"),
	//        rgx::Regex(R"(/robots.txt)"),
	//        rgx::Regex(R"(/phpmyadmin/)"),
	//        rgx::Regex(R"(/apple-touch-icon.*\.png)")
	//    };
	std::vector<re::Regex> IGNORABLE_404_URLS;

	// A secret key for this particular installation. Used in secret-key
	// hashing algorithms. Set this in your settings.
	std::string SECRET_KEY;

	// Absolute filesystem path to the directory that will hold user-uploaded files.
	//
	// Example: "/var/www/example.com/media/"
	std::string MEDIA_ROOT;

	// URL that handles the media served from MEDIA_ROOT.
	//
	// Examples: "http://example.com/media/", "http://media.example.com/"
	std::string MEDIA_URL;

	// Absolute path to the directory static files should be collected to.
	//
	// Example: "/var/www/example.com/static/"
	std::string STATIC_ROOT;

	// URL that handles the static files served from STATIC_ROOT.
	// Example: "http://example.com/static/", "http://static.example.com/"
	std::string STATIC_URL;

	// Maximum size, in bytes, of a request before it will be streamed to the
	// file system instead of into memory.
	size_t FILE_UPLOAD_MAX_MEMORY_SIZE = 2621440;

	// Maximum size in bytes of request data (excluding file uploads) that will be
	// read before a SuspiciousOperation (RequestDataTooBig) is raised.
	size_t DATA_UPLOAD_MAX_MEMORY_SIZE = 2621440;

	// Whether to prepend the "www." subdomain to URLs that don't have it.
	bool PREPEND_WWW = false;

	// Maximum number of GET/POST parameters that will be read before a
	// SuspiciousOperation (TooManyFieldsSent) is thrown.
	size_t DATA_UPLOAD_MAX_NUMBER_FIELDS = 1000;

	// Default formatting for date objects.
	std::string DATE_FORMAT = "%b %d, %Y";

	// Default formatting for datetime objects.
	std::string DATETIME_FORMAT = "%b %d, %Y, %T";

	// Default formatting for time objects.
	std::string TIME_FORMAT = "%T";

	// Default formatting for date objects when only the year and month are relevant.
	std::string YEAR_MONTH_FORMAT = "%B %Y";

	// Default formatting for date objects when only the month and day are relevant.
	std::string MONTH_DAY_FORMAT = "%B %d";

	// Default short formatting for date objects.
	std::string SHORT_DATE_FORMAT = "%m/%d/%Y";

	// Default short formatting for datetime objects.
	std::string SHORT_DATETIME_FORMAT = "%m/%d/%Y %T";

	// First day of week, to be used on calendars
	// 0 means Sunday, 1 means Monday...
	uint FIRST_DAY_OF_WEEK = 0;

	// Decimal separator symbol
	char DECIMAL_SEPARATOR = '.';

	// Boolean that sets whether to add thousand separator when formatting numbers
	bool USE_THOUSAND_SEPARATOR = false;

	// Thousand separator symbol
	char THOUSAND_SEPARATOR = ',';

	// Default X-Frame-Options header value
	std::string X_FRAME_OPTIONS = "SAMEORIGIN";

	bool USE_X_FORWARDED_HOST = false;
	bool USE_X_FORWARDED_PORT = false;

	// List of middleware to use. Order is important; in the request phase, these
	// middleware will be applied in the order given, and in the response
	// phase the middleware will be applied in reverse order.
	std::vector<std::shared_ptr<middleware::IMiddleware>> MIDDLEWARE;

	// Settings for CSRF cookie.
	std::string CSRF_COOKIE_NAME = "csrftoken";
	size_t CSRF_COOKIE_AGE = 60 * 60 * 24 * 7 * 52;
	std::string CSRF_COOKIE_DOMAIN;
	std::string CSRF_COOKIE_PATH = "/";
	bool CSRF_COOKIE_SECURE = false;
	bool CSRF_COOKIE_HTTP_ONLY = false;
	std::string CSRF_COOKIE_SAME_SITE = "Lax";
	std::string CSRF_HEADER_NAME = "X-XSRF-TOKEN";
	std::vector<std::string> CSRF_TRUSTED_ORIGINS;
	bool CSRF_USE_SESSIONS = false;

	// SSL settings (will be added in future).
	bool USE_SSL = false;

	// Security middleware.
	bool SECURE_BROWSER_XSS_FILTER = false;
	bool SECURE_CONTENT_TYPE_NO_SNIFF = true;
	bool SECURE_HSTS_INCLUDE_SUBDOMAINS = false;
	bool SECURE_HSTS_PRELOAD = false;
	size_t SECURE_HSTS_SECONDS = 0;
	std::vector<std::string> SECURE_REDIRECT_EXEMPT;
	std::string SECURE_REFERRER_POLICY;
	std::string SECURE_SSL_HOST;
	bool SECURE_SSL_REDIRECT = false;

	// If your module is behind a proxy that sets a header to specify secure
	// connections, AND that proxy ensures that user-submitted headers with the
	// same name are ignored (so that people can't spoof it), set this value to
	// a std::pair of (header_name, header_value). For any requests that come in with
	// that header/value, request.is_secure() will return true.
	// WARNING! Only set this if you fully understand what you're doing. Otherwise,
	// you may be opening yourself up to a security risk.
	std::shared_ptr<std::pair<std::string, std::string>> SECURE_PROXY_SSL_HEADER =
		std::make_shared<std::pair<std::string, std::string>>("X-Forwarded-Proto", "https");

	explicit Settings(const std::string& base_dir);
	virtual ~Settings() = default;
	void prepare();
	void perform_checks();

	inline virtual void register_modules()
	{
	}

	inline virtual void register_middleware()
	{
	}

	inline virtual void register_libraries()
	{
	}

	inline virtual void register_loaders()
	{
	}

	inline virtual void register_migrations()
	{
	}

	inline virtual std::shared_ptr<orm::abc::ISQLDriver> build_sqlite3_database(
		const std::string& name, const std::string& filepath
	)
	{
		std::shared_ptr<orm::abc::ISQLDriver> db;
		#ifdef USE_SQLITE3
			auto file_path = path::join(this->BASE_DIR, filepath);
			db = std::make_shared<orm::sqlite3::Driver>(file_path.c_str());
		#else
			db = nullptr;
		#endif
		return db;
	}

	inline virtual std::shared_ptr<orm::abc::ISQLDriver> build_database(
		const std::string& name, const YAML::Node& database
	)
	{
		return nullptr;
	}

	inline virtual std::shared_ptr<net::abc::IServer> build_server(const core::Kwargs& kwargs)
	{
		return nullptr;
	}

	[[nodiscard]]
	std::shared_ptr<IModuleConfig> get_module(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<middleware::IMiddleware> get_middleware(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<render::abc::ILibrary> get_library(const std::string& full_name) const;

	[[nodiscard]]
	std::shared_ptr<render::abc::ILoader> get_loader(const std::string& full_name) const;

	inline std::list<std::shared_ptr<orm::db::Migration>> get_migrations(orm::abc::ISQLDriver* driver)
	{
		if (this->_migrations.empty())
		{
			this->register_migrations();
		}

		std::list<std::shared_ptr<orm::db::Migration>> migrations;
		for (const auto& migration : this->_migrations)
		{
			migrations.push_back(migration(driver));
		}

		return migrations;
	}

	template <class T>
	[[nodiscard]]
	inline std::string get_name_or(const std::string& full_name) const
	{
		auto name = full_name;
		if (name.empty())
		{
			name = util::demangle(typeid(T).name());
		}

		return name;
	}

protected:
	template <module_config_type ModuleConfigT>
	inline void module(const std::string& custom_name="")
	{
		auto name = this->get_name_or<ModuleConfigT>(custom_name);
		if (this->_modules.find(name) != this->_modules.end() && this->LOGGER)
		{
			this->LOGGER->warning("module '" + name + "' was overwritten");
		}

		this->_modules[name] = [this, name]() -> std::shared_ptr<IModuleConfig> {
			auto module = std::make_shared<ModuleConfigT>(this);
			module->init(name);
			return module;
		};
	}

	template <middleware::middleware_type_c MiddlewareT>
	inline void middleware(const std::string& custom_name="")
	{
		auto name = this->get_name_or<MiddlewareT>(custom_name);
		if (this->_middleware.find(name) != this->_middleware.end() && this->LOGGER)
		{
			this->LOGGER->warning("middleware '" + name + "' was overwritten");
		}

		this->_middleware[name] = [this]() -> std::shared_ptr<middleware::IMiddleware> {
			return std::make_shared<MiddlewareT>(this);
		};
	}

	template <render::abc::library_type_c LibraryT>
	inline void library(const std::string& custom_name="")
	{
		auto name = this->get_name_or<LibraryT>(custom_name);
		if (this->_libraries.find(name) != this->_libraries.end() && this->LOGGER)
		{
			this->LOGGER->warning("library '" + name + "' was overwritten");
		}

		this->_libraries[name] = [this]() -> std::shared_ptr<render::abc::ILibrary> {
			return std::make_shared<LibraryT>(this);
		};
	}

	template <render::abc::loader_type_c LoaderT>
	inline void loader(const std::string& custom_name="")
	{
		auto name = this->get_name_or<LoaderT>(custom_name);
		if (this->_loaders.find(name) != this->_loaders.end() && this->LOGGER)
		{
			this->LOGGER->warning("loader '" + name + "' was overwritten");
		}

		this->_loaders[name] = [this]() -> std::shared_ptr<render::abc::ILoader> {
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
	std::map<
		std::string,
		std::function<std::shared_ptr<middleware::IMiddleware>()>
	> _middleware;

	std::map<
		std::string,
		std::function<std::shared_ptr<render::abc::ILibrary>()>
	> _libraries;

	std::map<
		std::string,
		std::function<std::shared_ptr<IModuleConfig>()>
	> _modules;

	std::map<
		std::string,
		std::function<std::shared_ptr<render::abc::ILoader>()>
	> _loaders;

	std::list<
		std::function<std::shared_ptr<orm::db::Migration>(orm::abc::ISQLDriver* driver)>
	> _migrations;
};

__CONF_END__
