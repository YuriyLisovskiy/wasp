/*
 * Copyright (c) 2019 Yuriy Lisovskiy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * settings
 * TODO: write docs.
 */

#ifndef WASP_CONF_SETTINGS_H
#define WASP_CONF_SETTINGS_H

// C++ libraries.
#include <string>
#include <vector>
#include <regex>

// Module definitions.
#include "./_def_.h"

// Wasp libraries.
#include "../utility/logger.h"
#include "../apps/config.h"
#include "../views/view.h"
#include "../middleware/middleware_mixin.h"


__CONF_BEGIN__

struct Settings
{
	bool DEBUG;

	wasp::utility::ILogger* LOGGER;

	// Build paths inside the project like this: wasp::path::join(BASE_DIR, ...)
	std::string BASE_DIR;

	// Hosts/domain names that are valid for this site.
	//
	// For example: "127.0.0.1" - matches localhost, "*" - matches all
	std::vector<std::string> ALLOWED_HOSTS;

	// Local time zone for this installation. All choices can be found here:
	// https://en.wikipedia.org/wiki/List_of_tz_zones_by_name (although not all
	// systems may support all possibilities). When USE_TZ is true, this is
	// interpreted as the default user time zone.
	std::string TIME_ZONE;

	// If you set this to True, Django will use timezone-aware datetimes.
	bool USE_TZ;

	// Default charset to use for all HttpResponse objects, if a MIME type isn't
	// manually specified. It's used to construct the Content-Type header.
	std::string DEFAULT_CHARSET;

	// Main AppConfig of an application.
	apps::AppConfig* ROOT_APP_CONFIG;

	// List of AppConfig-derived objects representing installed apps.
	std::vector<apps::AppConfig*> INSTALLED_APPS;

	// List of paths where templates can be found.
	std::vector<std::string> TEMPLATES;

	// Whether to append trailing slashes to URLs.
	bool APPEND_SLASH;

	// List of regular expression objects representing User-Agent strings
	// that are not allowed to visit any page, systemwide. Use this for bad
	// robots/crawlers.
	//
	// Here are a few examples:
	//     DISALLOWED_USER_AGENTS = {
	//         std::regex(R"(NaverBot.*)"),
	//         std::regex(R"(EmailSiphon.*)"),
	//         std::regex(R"(SiteSucker.*)"),
	//         std::regex(R"(sohu-search.*)")
	//     };
	std::vector<std::regex> DISALLOWED_USER_AGENTS;

	// List of compiled regular expression objects representing URLs that need not
	// be reported by BrokenLinkEmailsMiddleware.
	//
	// Here are a few examples:
	//    IGNORABLE_404_URLS = {
	//        std::regex(R"(/apple-touch-icon.*\.png)"),
	//        std::regex(R"(/favicon.ico)"),
	//        std::regex(R"(/robots.txt)"),
	//        std::regex(R"(/phpmyadmin/)"),
	//        std::regex(R"(/apple-touch-icon.*\.png)")
	//    };
	std::vector<std::regex> IGNORABLE_404_URLS;

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
	size_t FILE_UPLOAD_MAX_MEMORY_SIZE;

	// Maximum size in bytes of request data (excluding file uploads) that will be
	// read before a SuspiciousOperation (RequestDataTooBig) is raised.
	size_t DATA_UPLOAD_MAX_MEMORY_SIZE;

	// Maximum number of GET/POST parameters that will be read before a
	// SuspiciousOperation (TooManyFieldsSent) is thrown.
	size_t DATA_UPLOAD_MAX_NUMBER_FIELDS;

	// Default formatting for date objects.
	std::string DATE_FORMAT;

	// Default formatting for datetime objects.
	std::string DATETIME_FORMAT;

	// Default formatting for time objects.
	std::string TIME_FORMAT;

	// Default formatting for date objects when only the year and month are relevant.
	std::string YEAR_MONTH_FORMAT;

	// Default formatting for date objects when only the month and day are relevant.
	std::string MONTH_DAY_FORMAT;

	// Default short formatting for date objects.
	std::string SHORT_DATE_FORMAT;

	// Default short formatting for datetime objects.
	std::string SHORT_DATETIME_FORMAT;

	// First day of week, to be used on calendars
	// 0 means Sunday, 1 means Monday...
	uint FIRST_DAY_OF_WEEK;

	// Decimal separator symbol
	char DECIMAL_SEPARATOR;

	// Boolean that sets whether to add thousand separator when formatting numbers
	bool USE_THOUSAND_SEPARATOR;

	// Thousand separator symbol
	char THOUSAND_SEPARATOR;

	// Default X-Frame-Options header value
	std::string X_FRAME_OPTIONS;

	bool USE_X_FORWARDED_HOST;
	bool USE_X_FORWARDED_PORT;

	//# List of middleware to use. Order is important; in the request phase, these
	//# middleware will be applied in the order given, and in the response
	//# phase the middleware will be applied in reverse order.
	std::vector<middleware::MiddlewareMixin*> MIDDLEWARE;

	// Used as view when a request is rejected by the CSRF middleware.
	views::View* CSRF_FAILURE_VIEW;

	// Settings for CSRF cookie.
	std::string CSRF_COOKIE_NAME;
	size_t CSRF_COOKIE_AGE;
	std::string CSRF_COOKIE_DOMAIN;
	std::string CSRF_COOKIE_PATH;
	bool CSRF_COOKIE_SECURE;
	bool CSRF_COOKIE_HTTP_ONLY;
	std::string CSRF_COOKIE_SAME_SITE;
	std::string CSRF_HEADER_NAME;
	std::vector<std::string> CSRF_TRUSTED_ORIGINS;
	bool CSRF_USE_SESSIONS;

	Settings();
	virtual ~Settings();

	// TODO:
	template <typename _T>
	apps::AppConfig* app()
	{
		return new _T();
	}

	// TODO:
	template <typename _T>
	middleware::MiddlewareMixin* middleware()
	{
		return new _T();
	}

	// TODO:
	template <typename _T>
	views::View* view()
	{
		return new _T();
	}
};

__CONF_END__


#endif // WASP_CONF_SETTINGS_H
