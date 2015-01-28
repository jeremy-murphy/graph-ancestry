/*
    Copyright (C) 2014  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file test_DAG.hpp
 * @brief Unit tests for DAG functions.  
 */

#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "DAG.hpp"
#include "fixture.hpp"

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

#include <boost/graph/isomorphism.hpp>

#include <iostream>
#include <locale>

using namespace graph_algorithms;

struct enable_locale
{
    enable_locale() { std::cout.imbue(std::locale("")); std::cerr.imbue(std::locale("")); }
};



BOOST_FIXTURE_TEST_CASE(test_find_common_ancestor_single_use, Bender_2005_4<>)
{
#ifndef NDEBUG
    std::ofstream output("Bender_2005_4.dot");
    boost::write_graphviz(output, g);
#endif
    auto const V = boost::vertices(g);
    auto result = find_common_ancestor(g, 0u, std::next(V.first, 6), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 8);
    result = find_common_ancestor(g, 5u, std::next(V.first, 6), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 8);
    result = find_common_ancestor(g, 5u, std::next(V.first, 3), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 5);
    result = find_common_ancestor(g, 1u, std::next(V.first, 2), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 6);
    result = find_common_ancestor(g, 8u, std::next(V.first, 0), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 0);
    result = find_common_ancestor(g, 8u, std::next(V.first, 5), V.second);
    BOOST_CHECK_EQUAL(std::distance(V.first, result.first), 5);
}

BOOST_FIXTURE_TEST_CASE(test_find_common_ancestor_repeat_use, Bender_2005_4<>)
{
    using namespace boost;
    
    typedef typename graph_traits<decltype(g)>::vertex_descriptor vertex_descriptor;
    
    auto const V = vertices(g);
    // std::unordered_map<vertex_descriptor, default_color_type> target_colour, search_colour;
    // typedef associative_property_map<decltype(target_colour)> ColourPropertyMap;
    vector_property_map<default_color_type> ancestors(num_vertices(g)), searched(num_vertices(g));
    std::for_each(V.first, V.second, [&](vertex_descriptor u)
    {
        put(ancestors, u, white_color);
        put(searched, u, white_color);
    });
    queue<vertex_descriptor> q;
    std::unordered_map<vertex_descriptor, vertex_descriptor> predecessor;
    auto const H = make_reverse_graph(g);
    breadth_first_visit(H, 6u, q, default_bfs_visitor(), ancestors);

    auto relative = find_common_ancestor_impl(H, std::next(V.first, 5), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 6);
    relative = find_common_ancestor_impl(H, std::next(relative.first), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 7);
    relative = find_common_ancestor_impl(H, std::next(relative.first), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 8);
    relative = find_common_ancestor_impl(H, std::next(relative.first), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 9);
    // Reinitialize.
    std::for_each(V.first, V.second, [&](vertex_descriptor u)
    {
        put(ancestors, u, white_color);
        put(searched, u, white_color);
    });
    breadth_first_visit(H, 7u, q, default_bfs_visitor(), ancestors);
    relative = find_common_ancestor_impl(H, std::next(V.first, 8), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 8);
    relative = find_common_ancestor_impl(H, std::next(relative.first), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 9);
    relative = find_common_ancestor_impl(H, std::next(relative.first), V.second, ancestors, searched, q, predecessor);
    BOOST_REQUIRE_EQUAL(std::distance(V.first, relative.first), 10);
}
