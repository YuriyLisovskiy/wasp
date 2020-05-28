/*
 * Copyright (c) 2020 Yuriy Lisovskiy
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
 * An implementation of render/internal/syntax/foreach_tag.h
 */

// C++ libraries.
//#include <vector>

// Header.
#include "./foreach_tag.h"

// Framework modules.
#include "../utility.h"
#include "../../../core/types/array.h"
#include "../../../core/exceptions.h"


__SYNTAX_BEGIN__

class ForEachLoop : public core::object::Object
{
public:
	core::types::Value<long long> counter;
	core::types::Value<long long> counter0;
	core::types::Value<long long> rev_counter;
	core::types::Value<long long> rev_counter0;
	core::types::Value<bool> first;
	core::types::Value<bool> last;
	core::types::Value<std::shared_ptr<ForEachLoop>> parent_loop;

	ForEachLoop()
	{
		using Attr = core::object::Attr;
		this->__attrs__ = {
			{"counter", Attr([this]() -> Object* { return &this->counter; })},
			{"counter0", Attr([this]() -> Object* { return &this->counter0; })},
			{"rev_counter", Attr([this]() -> Object* { return &this->rev_counter; })},
			{"rev_counter0", Attr([this]() -> Object* { return &this->rev_counter0; })},
			{"first", Attr([this]() -> Object* { return &this->first; })},
			{"last", Attr([this]() -> Object* { return &this->last; })},
		};
	}

	ForEachLoop(const ForEachLoop& other) : Object(other)
	{
		if (this != &other)
		{
			this->counter = other.counter;
			this->counter0 = other.counter0;
			this->rev_counter = other.rev_counter;
			this->rev_counter0 = other.counter0;
			this->first = other.first;
			this->last = other.last;
			auto pl_ptr = other.parent_loop.get();
			if (pl_ptr)
			{
				this->parent_loop = core::types::Value<std::shared_ptr<ForEachLoop>>(
					std::make_shared<ForEachLoop>(*pl_ptr)
				);
			}

			using Attr = core::object::Attr;
			this->__attrs__ = {
				{"counter", Attr([this]() -> Object* { return &this->counter; })},
				{"counter0", Attr([this]() -> Object* { return &this->counter0; })},
				{"rev_counter", Attr([this]() -> Object* { return &this->rev_counter; })},
				{"rev_counter0", Attr([this]() -> Object* { return &this->rev_counter0; })},
				{"first", Attr([this]() -> Object* { return &this->first; })},
				{"last", Attr([this]() -> Object* { return &this->last; })},
			};
		}
	}
};

std::string foreach_node::render(IContext* ctx)
{
	using Array = core::types::Array;

	auto fl_ptr = ctx->find_var("foreach_loop");
	ForEachLoop* parent_loop = nullptr;
	if (fl_ptr)
	{
		parent_loop = dynamic_cast<ForEachLoop*>(fl_ptr.get());
	}

	auto values_ptr = this->sequence->resolve(ctx);
	std::shared_ptr<Array> values;
	if (values_ptr)
	{
		values = std::make_shared<Array>(*dynamic_cast<Array*>(values_ptr.get()));
	}
	else
	{
		values = std::make_shared<Array>();
	}

	auto len_values = values->size();
	if (len_values < 1)
	{
		return this->nodelist_empty->render(ctx);
	}

	if (this->is_reversed)
	{
		values->reverse();
	}

	std::string rendered_result;
	auto num_loop_vars = this->loop_vars.size();
	auto unpack = num_loop_vars > 1;
	ctx->push_scope({});

	// Create a foreach_loop value in the context.
	// We'll update counters on each iteration just below.
	auto foreach_loop = std::make_shared<ForEachLoop>();
	if (parent_loop)
	{
		foreach_loop->parent_loop = std::make_shared<ForEachLoop>(*parent_loop);
	}

	for (size_t i = 0; i < len_values; i++)
	{
		// Shortcuts for current loop iteration number.
		foreach_loop->counter0 = i;
		foreach_loop->counter = i + 1;

		// Reverse counter iteration numbers.
		foreach_loop->rev_counter = len_values - i;
		foreach_loop->rev_counter0 = len_values - i - 1;

		// Boolean values designating first and last times through loop.
		foreach_loop->first = (i == 0);
		foreach_loop->last = (i == (len_values - 1));

		auto pop_context = false;
		if (unpack)
		{
			if (num_loop_vars > 2)
			{
				throw core::ValueError(
					"Need 2 values to unpack in for loop; got " + std::to_string(num_loop_vars) + "."
				);
			}

			pop_context = true;
			ctx->push_scope({
				{this->loop_vars[0].content, std::make_shared<core::types::Value<size_t>>(i)},
				{this->loop_vars[1].content, values->at(i)}
			});
		}
		else
		{
			ctx->push_var(this->loop_vars[0].content, values->at(i));
		}

		rendered_result += this->nodelist_loop->render(ctx);
		if (pop_context)
		{
			// Pop the loop variables pushed on to the context to avoid
			// the context ending up in an inconsistent state when other
			// tags push data to context.
			ctx->pop_scope();
		}
	}

	ctx->push_var("foreach_loop", foreach_loop);
	return rendered_result;
}

std::shared_ptr<internal::node> parse_foreach(
	internal::parser* parser,
	internal::token_t& token
)
{
	auto curr_pos = token.content.find(TAG_NAME_FOREACH) + TAG_NAME_FOREACH.size();
	auto open_bracket_pos = token.content.find('(', curr_pos);
	if (open_bracket_pos == std::string::npos)
	{
		parser::throw_error(
			"missing opening bracket: " + token.content,
			token, token.line_no
		);
	}

	if (*(token.content.end() - 1) != ')')
	{
		parser::throw_error(
			"missing closing bracket: " + token.content,
			token, token.line_no
		);
	}

	auto content = token.content.substr(0, token.content.size() - 1);
	curr_pos = open_bracket_pos + 1;
	auto colon_pos = content.find(':', curr_pos);
	if (colon_pos == std::string::npos)
	{
		parser::throw_error(
			"incorrect '" + TAG_NAME_FOREACH + "' statement, use the format 'foreach(x : y)': " + token.content,
			token, token.line_no
		);
	}

	foreach_node fnd{};

	// retrieve vars, i.e. ``i, item``
	if (!split_for_loop_vars(
		content, token.line_no,
		curr_pos, fnd.loop_vars, 2
	))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	curr_pos = colon_pos;

	// retrieve container variables
	std::vector<token_t> sequence_var;
	if (!split_for_loop_vars(
		content, token.line_no,
		++curr_pos, sequence_var, 1
	))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	if (sequence_var.size() != 1)
	{
		parser::invalid_syntax(token, curr_pos);
	}

	fnd.sequence = parser->compile_filter(sequence_var[0]);

	// identify if reverse container items
	std::vector<token_t> reversed_sig;
	if (!split_for_loop_vars(
		content, token.line_no,
		++curr_pos, reversed_sig, 1
	))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	auto rev_sig_size = reversed_sig.size();
	fnd.is_reversed = false;
	if (rev_sig_size > 0)
	{
		if (rev_sig_size != 1)
		{
			parser::invalid_syntax(token, curr_pos);
		}

		if (reversed_sig[0].content != "reversed")
		{
			parser::invalid_syntax(token, curr_pos);
		}

		fnd.is_reversed = true;
	}

	fnd.nodelist_loop = parser->parse(
		{"empty", "end_" + TAG_NAME_FOREACH}
	);
	auto next_token = parser->next_token();
	if (next_token.content == "empty")
	{
		fnd.nodelist_empty = parser->parse({"end_" + TAG_NAME_FOREACH});
		parser->del_first_token();
	}
	else
	{
		fnd.nodelist_empty = std::make_shared<node_list>();
	}

	return std::make_shared<foreach_node>(fnd);
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_foreach_tag()
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		return parse_foreach(parser, token);
	};
}

__SYNTAX_END__
