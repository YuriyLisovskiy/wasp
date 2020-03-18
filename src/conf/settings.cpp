/*
 * Copyright (c) 2019-2020 Yuriy Lisovskiy
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

/**
 * An implementation of conf/settings.h
 */

#include "./settings.h"


__CONF_BEGIN__

Settings::Settings()
{
	this->DEBUG = false;

	this->LOGGER = core::Logger::get_instance({});

	this->TIME_ZONE = "America/Chicago";

	this->USE_TZ = false;

	this->DEFAULT_CHARSET = "utf-8";

	this->ROOT_APP = nullptr;

	this->TEMPLATES_BACKEND = nullptr;

	this->APPEND_SLASH = true;

	this->FILE_UPLOAD_MAX_MEMORY_SIZE = 2621440;   // 2.5 MB

	this->DATA_UPLOAD_MAX_MEMORY_SIZE = 2621440;   // 2.5 MB

	this->DATA_UPLOAD_MAX_NUMBER_FIELDS = 1000;

	this->PREPEND_WWW = false;

	this->DATE_FORMAT = "%b %d, %Y";

	this->DATETIME_FORMAT = "%b %d, %Y, %T";

	this->TIME_FORMAT = "%T";

	this->YEAR_MONTH_FORMAT = "%B %Y";

	this->MONTH_DAY_FORMAT = "%B %d";

	this->SHORT_DATE_FORMAT = "%m/%d/%Y";

	this->SHORT_DATETIME_FORMAT = "%m/%d/%Y %T";

	this->FIRST_DAY_OF_WEEK = 0;

	this->DECIMAL_SEPARATOR = '.';

	this->USE_THOUSAND_SEPARATOR = false;

	this->THOUSAND_SEPARATOR = ',';

	this->X_FRAME_OPTIONS = "SAMEORIGIN";

	this->USE_X_FORWARDED_HOST = false;
	this->USE_X_FORWARDED_PORT = false;

	this->SECURE_PROXY_SSL_HEADER = nullptr;

	this->CSRF_COOKIE_NAME = "csrftoken";
	this->CSRF_COOKIE_AGE = 60 * 60 * 24 * 7 * 52;
	this->CSRF_COOKIE_DOMAIN = "";
	this->CSRF_COOKIE_PATH = "/";
	this->CSRF_COOKIE_SECURE = false;
	this->CSRF_COOKIE_HTTP_ONLY = false;
	this->CSRF_COOKIE_SAME_SITE = "Lax";
	this->CSRF_HEADER_NAME = "HTTP_X_CSRFTOKEN";
	this->CSRF_USE_SESSIONS = false;

	this->USE_SSL = false;

	this->SECURE_BROWSER_XSS_FILTER = false;
	this->SECURE_CONTENT_TYPE_NO_SNIFF = true;
	this->SECURE_HSTS_INCLUDE_SUBDOMAINS = false;
	this->SECURE_HSTS_PRELOAD = false;
	this->SECURE_HSTS_SECONDS = 0;
	this->SECURE_SSL_REDIRECT = false;
}

void Settings::override()
{
}

void Settings::prepare()
{
	if (!this->ROOT_APP && !this->INSTALLED_APPS.empty())
	{
		this->ROOT_APP = this->INSTALLED_APPS.front();
	}
}

Settings::~Settings()
{
	delete this->TEMPLATES_BACKEND;
	delete this->SECURE_PROXY_SSL_HEADER;
//	for (auto& installed_app : this->INSTALLED_APPS)
//	{
//		delete installed_app;
//	}

	for (auto& middleware : this->MIDDLEWARE)
	{
		delete middleware;
	}

	for (auto& command : this->COMMANDS)
	{
		delete command.second;
	}

	core::Logger::reset_instance();
}

__CONF_END__
