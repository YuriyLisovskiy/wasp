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
 * An implementation of render/internal/syntax/for_tag.h
 */

// C++ libraries.
#include <vector>

// Header.
#include "./for_tag.h"

// Framework modules.
#include "../utility.h"
#include "../../../core/types/array.h"


__SYNTAX_BEGIN__

class ForLoop : public core::object::Object
{
public:
	core::types::Value<long long> counter;
	core::types::Value<long long> counter0;
	core::types::Value<long long> rev_counter;
	core::types::Value<long long> rev_counter0;
	core::types::Value<bool> first;
	core::types::Value<bool> last;
	core::types::Value<std::shared_ptr<ForLoop>> parent_loop;

	ForLoop()
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

	ForLoop(const ForLoop& other) : Object(other)
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
				this->parent_loop = core::types::Value<std::shared_ptr<ForLoop>>(
					std::make_shared<ForLoop>(*pl_ptr)
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

std::string for_node::render(IContext* ctx)
{
	using Array = core::types::Array;

	auto fl_ptr = ctx->find_var("for_loop");
	ForLoop* parent_loop = nullptr;
	if (fl_ptr)
	{
		parent_loop = dynamic_cast<ForLoop*>(fl_ptr.get());
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

	// Create a for_loop value in the context.
	// We'll update counters on each iteration just below.
	auto for_loop = std::make_shared<ForLoop>();
	if (parent_loop)
	{
		for_loop->parent_loop = std::make_shared<ForLoop>(*parent_loop);
	}

	for (size_t i = 0; i < len_values; i++)
	{
		// Shortcuts for current loop iteration number.
		for_loop->counter0 = i;
		for_loop->counter = i + 1;

		// Reverse counter iteration numbers.
		for_loop->rev_counter = len_values - i;
		for_loop->rev_counter0 = len_values - i - 1;

		// Boolean values designating first and last times through loop.
		for_loop->first = (i == 0);
		for_loop->last = (i == (len_values - 1));

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

	ctx->push_var("for_loop", for_loop);
	return rendered_result;
}

std::shared_ptr<internal::node> parse_for(
	internal::parser* parser,
	internal::token_t& token
)
{
	auto curr_pos = token.content.find("for") + 3;
	auto colon_pos = token.content.find(':', curr_pos);
	if (colon_pos == std::string::npos)
	{
		parser::throw_error(
			"incorrect 'for' statement, use the format 'for x : y': " + token.content,
			token, token.line_no
		);
	}

	for_node fnd{};

	// retrieve vars, i.e. ``i, item``
	if (!split_for_loop_vars(
		token.content, token.line_no,
		curr_pos, fnd.loop_vars, 2
	))
	{
		parser::invalid_syntax(token, curr_pos);
	}

	curr_pos = colon_pos;

	// retrieve container variables
	std::vector<token_t> sequence_var;
	if (!split_for_loop_vars(
		token.content,
		token.line_no, ++curr_pos, sequence_var, 1
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
		token.content,
		token.line_no, ++curr_pos, reversed_sig, 1
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
		{"empty", "end_" + TAG_NAME_FOR}
	);
	auto next_token = parser->next_token();
	if (next_token.content == "empty")
	{
		fnd.nodelist_empty = parser->parse({"end_" + TAG_NAME_FOR});
		parser->del_first_token();
	}
	else
	{
		fnd.nodelist_empty = std::make_shared<node_list>();
	}

	return std::make_shared<for_node>(fnd);
}

std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_for_tag()
{
	return [](
		internal::parser* parser,
		internal::token_t& token
	) -> std::shared_ptr<internal::node>
	{
		return parse_for(parser, token);
	};
}

__SYNTAX_END__
