/**
 * render/functions.cpp
 *
 * Copyright (c) 2021 Yuriy Lisovskiy
 */

#include "./functions.h"

// C++ libraries.
#include <vector>
#include <string>

// Base libraries.
#include <xalwart.base/exceptions.h>
#include <xalwart.base/path.h>
#include <xalwart.base/types/string.h>


__RENDER_BEGIN__

abc::render::ILibrary::Function make_static_function(const std::string& prefix)
{
	return [prefix](
		abc::render::IContext* context,
		const std::vector<abc::render::ILibrary::Argument>& arguments,
		const std::optional<std::string>& result_variable,
		size_t function_call_line
	) -> std::string
	{
		auto arguments_count = arguments.size();
		if (arguments_count != 1)
		{
			throw TemplateError(
				"Function 'static' accepts exactly one arguments, got " + std::to_string(arguments_count) +
				", function is called at line " + std::to_string(function_call_line)
			);
		}

		auto path_argument = arguments.front();
		std::string result_path = path::join(prefix, path_argument ? path_argument->__str__() : "");

		if (result_variable.has_value())
		{
			context->push_var(result_variable.value(), std::make_shared<types::String>(result_path));
			result_path = "";
		}

		return result_path;
	};
}

abc::render::ILibrary::Function make_url_function(const std::vector<std::shared_ptr<urls::IPattern>>& patterns)
{
	return [patterns](
		abc::render::IContext* context,
		const std::vector<abc::render::ILibrary::Argument>& arguments,
		const std::optional<std::string>& result_variable,
		size_t function_call_line
	) -> std::string
	{
		if (arguments.empty())
		{
			throw TemplateError(
				"Function 'url' accepts at least one argument, function is called at line " +
				std::to_string(function_call_line)
			);
		}

		auto pattern_name = arguments.front() ? arguments.front()->__str__() : "";
		std::string pattern_string;
		for (const auto& pattern : patterns)
		{
			if (pattern->get_name() == pattern_name)
			{
				std::vector<std::string> pattern_arguments;
				auto patterns_arguments_count = arguments.size() - 1;
				if (patterns_arguments_count > 0)
				{
					pattern_arguments.reserve(patterns_arguments_count);
					std::transform(
						arguments.begin() + 1,  arguments.end(), pattern_arguments.begin(),
						[](const auto& object) -> std::string { return object ? object->__str__() : ""; }
					);
				}
				pattern_string = pattern->build(pattern_arguments);
				break;
			}
		}

		if (pattern_string.empty())
		{
			throw TemplateError(
				"URL pattern '" + pattern_name + "' does not exist, function is called at line " +
				std::to_string(function_call_line)
			);
		}

		if (result_variable.has_value())
		{
			context->push_var(result_variable.value(), std::make_shared<types::String>(pattern_string));
			pattern_string = "";
		}

		return pattern_string;
	};
}

__RENDER_END__
