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

#include "./logger.h"


__UTILITY_BEGIN__

ILogger* Logger::_instance = nullptr;

Logger::~Logger()
{
	delete Logger::_instance;
	Logger::_instance = nullptr;
}

ILogger* Logger::get_instance(const Logger::Config& cfg)
{
	if (Logger::_instance == nullptr)
	{
		Logger::_instance = new Logger(cfg);
	}

	return Logger::_instance;
}

Logger::Logger(const Logger::Config& cfg)
{
	this->_config = cfg;
	if (cfg.streams.empty())
	{
		this->_config.streams.emplace_back(&std::cout);
	}
}

void Logger::info(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_info);
}

void Logger::debug(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_debug);
}

void Logger::warning(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_warning);
}

void Logger::error(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_error);
}

void Logger::fatal(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_fatal);
}

void Logger::trace(const std::string& msg, int line, const char* function, const char* file)
{
	this->log(msg, line, function, file, Logger::log_level_enum::ll_trace);
}

void Logger::trace(const char* msg, int line, const char* function, const char* file)
{
	this->trace(std::string(msg), line, function, file);
}

void Logger::log(const std::string& msg, int line, const char* function, const char* file, Logger::log_level_enum level)
{
	std::string level_name;
	bool is_enabled;
	const char* colour;
	switch (level)
	{
		case Logger::log_level_enum::ll_warning:
			level_name = "[warning]";
			is_enabled = this->_config.log_warning;
			colour = Logger::YELLOW;
			break;
		case Logger::log_level_enum::ll_error:
			level_name = "[error]";
			is_enabled = this->_config.log_error;
			colour = Logger::RED;
			break;
		case Logger::log_level_enum::ll_debug:
			level_name = "[debug]";
			is_enabled = this->_config.log_debug;
			colour = Logger::MAGENTA;
			break;
		case Logger::log_level_enum::ll_fatal:
			level_name = "[fatal]";
			is_enabled = this->_config.log_fatal;
			colour = Logger::BOLD_RED;
			break;
		case Logger::log_level_enum::ll_trace:
			level_name = "[trace]";
			is_enabled = this->_config.log_trace;
			colour = Logger::RED;
			break;
		default:
			level_name = "[info]";
			is_enabled = this->_config.log_info;
			colour = Logger::CYAN;
			break;
	}
	if (!is_enabled)
	{
		return;
	}

	std::string full_msg;
	if (line != 0 && std::strlen(file) > 0 && std::strlen(function) > 0)
	{
		full_msg = "\tFile \"" + std::string(file) + "\", line "
			+ std::to_string(line) + ", in "
			+ std::string(function) + "\n" + msg;
	}
	else
	{
		full_msg = msg;
	}

	std::string result = "[" + dt::now().strftime("%F %T") +
		"] " + level_name + ":\n" + full_msg + "\n";
	this->write_to_stream(result, colour);
}

void Logger::write_to_stream(const std::string& msg, const char* colour)
{
	for (auto& stream : this->_config.streams)
	{
		*stream << '\n' << colour << msg << Logger::DEFAULT << '\n';
	}

	this->flush();
}

void Logger::flush()
{
	for (auto& stream : this->_config.streams)
	{
		stream->flush();
	}
}

__UTILITY_END__
