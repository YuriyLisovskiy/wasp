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
 * for_tag.h
 *
 * Purpose:
 *  TODO: implement docs for for_tag.h
 */

#pragma once

// C++ libraries.
// TODO:

// Module definitions.
#include "./_def_.h"

// Framework modules.
#include "../nodes.h"
#include "../parser.h"


__SYNTAX_BEGIN__

const std::string TAG_NAME_FOR = "for";

struct for_node : public node
{
	std::vector<token_t> loop_vars;
	std::shared_ptr<FilterExpression> sequence;
	bool is_reversed;
	std::shared_ptr<node_list> nodelist_loop;
	std::shared_ptr<node_list> nodelist_empty;

	std::string render(IContext* ctx) override;
};

// Loop over each item in an array.
// For example, to display a list of athletes given ``athlete_list``:

//     <ul>
//         {% for athlete : athlete_list %}
//             <li>{{ athlete.name }}</li>
//         {% end_for %}
//     </ul>
//
// You can loop over a list in reverse by using
// ``{% for obj : list reversed %}``.
//
// You can also unpack multiple values from a two-dimensional array:
//
//     {% for key,value in dict.items %}
//         {{ key }}: {{ value }}
//     {% end_for %}
//
// The ``for`` tag can take an optional ``{% empty %}`` clause that will
// be displayed if the given array is empty or could not be found:
//
//     <ul>
//         {% for athlete : athlete_list %}
//             <li>{{ athlete.name }}</li>
//         {% empty %}
//             <li>Sorry, no athletes in this list.</li>
//         {% end_for %}
//     <ul>
//
// The for loop sets a number of variables available within the loop:
//     ==========================  ================================================
//     Variable                    Description
//     ==========================  ================================================
//     ``for_loop.counter``        The current iteration of the loop (1-indexed)
//     ``for_loop.counter0``       The current iteration of the loop (0-indexed)
//     ``for_loop.rev_counter``    The number of iterations from the end of the
//                                 loop (1-indexed)
//     ``for_loop.rev_counter0``   The number of iterations from the end of the
//                                 loop (0-indexed)
//     ``for_loop.first``          True if this is the first time through the loop
//     ``for_loop.last``           True if this is the last time through the loop
//     ``for_loop.parent_loop``    For nested loops, this is the loop "above" the
//                                 current one
//     ==========================  ================================================
extern std::shared_ptr<internal::node> parse_for(
	internal::parser* parser,
	internal::token_t& token
);

extern std::function<std::shared_ptr<internal::node>(
	internal::parser*, internal::token_t& token
)> make_for_tag();

__SYNTAX_END__
