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

#ifndef LEAF_UTILS_LOGGER_H
#define LEAF_UTILS_LOGGER_H

#include <iostream>
#include <string>
#include <ostream>
#include <ctime>
#include <locale>

#include "../globals.h"


__LEAF_BEGIN__

class ILogger
{
public:
	virtual ~ILogger() = default;
	virtual void info(const std::string& msg) = 0;
	virtual void debug(const std::string& msg) = 0;
	virtual void warning(const std::string& msg) = 0;
	virtual void error(const std::string& msg) = 0;
	virtual void trace(const std::string& msg, const char* file, const char* function, int line) = 0;
};

class Logger : public ILogger
{
public:
	static ILogger* getInstance();

	void info(const std::string& msg) override;
	void debug(const std::string& msg) override;
	void warning(const std::string& msg) override;
	void error(const std::string& msg) override;
	void trace(const std::string& msg, const char* file, const char* function, int line) override;

private:
	enum LogLevel
	{
		Info, Debug, Warning, Error, Fatal
	};

	static ILogger* instance;

	Logger() = default;
	~Logger() override;
	void log(const std::string& msg, LogLevel logLevel);
	void writeToStream(const std::string& msg);
};

__LEAF_END__


#endif // LEAF_UTILS_LOGGER_H
