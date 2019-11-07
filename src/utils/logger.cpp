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

#include "logger.h"


__WASP_BEGIN__

ILogger* Logger::instance = nullptr;

Logger::~Logger()
{
	delete Logger::instance;
	Logger::instance = nullptr;
}

ILogger* Logger::getInstance()
{
	if (Logger::instance == nullptr)
	{
		Logger::instance = new Logger();
	}

	return Logger::instance;
}

void Logger::info(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, LogLevel::Info);
}

void Logger::debug(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, LogLevel::Debug);
}

void Logger::warning(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, LogLevel::Warning);
}

void Logger::error(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, LogLevel::Error);
}

void Logger::trace(const std::string& msg, int line, const char* function, const char* file)
{
	this->writeToStream("Traceback (exception):\n  File \"" + std::string(file) + "\", line " + std::to_string(line) + ", in " + std::string(function) + "\n" + msg + "\n", LogLevel::Error);
}

void Logger::trace(const char* msg, int line, const char* function, const char* file)
{
	this->trace(std::string(msg), line, function, file);
}

void Logger::log(const std::string& msg, int line, const char* function, const char* file, Logger::LogLevel logLevel)
{
	std::string level;
	switch (logLevel)
	{
		case LogLevel::Warning:
			level = "[warning]";
			break;
		case LogLevel::Error:
			level = "[error]";
			break;
		case LogLevel::Debug:
			level = "[debug]";
			break;
		case LogLevel::Fatal:
			level = "[fatal]";
			break;
		default:
			level = "[info]";
			break;
	}
	std::locale::global(std::locale("uk_UA.utf8"));
	std::time_t t = std::time(nullptr);
	char now[100];
	std::strftime(now, sizeof(now), "%F %T", std::localtime(&t));

	std::string fullMsg = "\tFile \"" + std::string(file) + "\", line " + std::to_string(line) + ", in " + std::string(function) + "\n" + msg;

	this->writeToStream("[" + std::string(now) + "] [" + "thread id" + "] " + level + ":\n" + fullMsg + "\n", logLevel);
}

void Logger::writeToStream(const std::string& msg, LogLevel level)
{
	// TODO: add multiple streams support
	std::cout << '\n' << msg;
	std::cout.flush();
}

__WASP_END__
