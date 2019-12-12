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

#ifndef WASP_UTILS_LOGGER_H
#define WASP_UTILS_LOGGER_H

#include <iostream>
#include <string>
#include <ostream>
#include <ctime>
#include <locale>
#include <cstring>

#include "../globals.h"


__WASP_BEGIN__

class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void info(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void debug(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void warning(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void error(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void fatal(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void trace(const std::string& msg, int line, const char* function, const char* file) = 0;
	virtual void trace(const char* msg, int line, const char* function, const char* file) = 0;
};

class Logger : public ILogger
{
public:
	static ILogger* get_instance();

	void info(const std::string& msg, int line, const char* function, const char* file) override;
	void debug(const std::string& msg, int line, const char* function, const char* file) override;
	void warning(const std::string& msg, int line, const char* function, const char* file) override;
	void error(const std::string& msg, int line, const char* function, const char* file) override;
	void fatal(const std::string& msg, int line, const char* function, const char* file) override;
	void trace(const std::string& msg, int line, const char* function, const char* file) override;
	void trace(const char* msg, int line, const char* function, const char* file) override;

private:
	enum log_level_enum
	{
		ll_info, ll_debug, ll_warning, ll_error, ll_fatal
	};

	static ILogger* _instance;

	Logger() = default;
	~Logger() override;
	void log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level);
	void write_to_stream(const std::string& msg, Logger::log_level_enum level);
};

__WASP_END__


#endif // WASP_UTILS_LOGGER_H
