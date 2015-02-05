/*
    Copyright (C) 2015  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>

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
 * @file test_reflect.hpp
 * @brief Unit tests for reflection functions.  
 */

#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "reflect.hpp"
#include "fixture.hpp"

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/copy.hpp>
#include <boost/graph/isomorphism.hpp>
#include <boost/graph/transitive_closure.hpp>

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

#include <iostream>
#include <locale>

using namespace graph_algorithms;

struct enable_locale
{
    enable_locale() { std::cout.imbue(std::locale("")); std::cerr.imbue(std::locale("")); }
};

BOOST_FIXTURE_TEST_SUITE(Reflection, Bender_2005_4<DefaultAdjacencyList>)

BOOST_AUTO_TEST_CASE(test_reflect_through_sources_1)
{
    #ifndef NDEBUG
    {
        std::ofstream output("Bender_2005_4.dot");
        boost::write_graphviz(output, g);
    }
    #endif
    reflect_through_sources(g);
    #ifndef NDEBUG
    {
        std::ofstream output("Bender_2005_4_F.dot");
        boost::write_graphviz(output, g);
    }
    #endif
    DefaultAdjacencyList h;
    add_Bender_2005_4_F(h);
    BOOST_CHECK_EQUAL(boost::num_vertices(g), boost::num_vertices(h));
    BOOST_REQUIRE_EQUAL(boost::num_edges(g), boost::num_edges(h));
    BOOST_REQUIRE(boost::isomorphism(g, h));
}


BOOST_AUTO_TEST_CASE(test_reflect_through_sources_2)
{
    typedef boost::graph_traits<decltype(g)>::vertex_descriptor vertex_descriptor;
    typedef boost::graph_traits<decltype(g)>::edge_descriptor edge_descriptor;
    
    auto const V = vertices(g);
    std::unordered_map<vertex_descriptor, boost::default_color_type> vertex_color;
    boost::associative_property_map<decltype(vertex_color)> colour(vertex_color);
    boost::queue<vertex_descriptor> q;
    
    std::for_each(V.first, V.second, [&](vertex_descriptor u){ put(colour, u, boost::white_color); });
    
    boost::adjacency_matrix<> h(17);
#ifndef NDEBUG
    {
        std::ofstream output("Bender_2005_4_matrix.dot");
        boost::write_graphviz(output, h);
    }
#endif
    reflect_through_sources(g, h);
    DefaultAdjacencyList F;
    add_Bender_2005_4_F(F);
    BOOST_CHECK_EQUAL(num_vertices(h), num_vertices(F));
    BOOST_REQUIRE_EQUAL(num_edges(h), num_edges(F));
    BOOST_REQUIRE(boost::isomorphism(h, F));
}


BOOST_AUTO_TEST_CASE(test_transitive_closure_of_reflected_graph)
{
    boost::adjacency_matrix<> TC_F(17);
    auto offset = create_common_ancestor_existence_tc(g, TC_F);
    // Now that we finally have a transitive-closure of F, we can query.
    auto q = edge(5 + offset, 9, TC_F);
    BOOST_CHECK(q.second);
    BOOST_CHECK(!does_common_ancestor_exist(5, 10, TC_F, offset));
    q = edge(8 + offset, 10, TC_F);
    BOOST_CHECK(q.second);
    q = edge(5 + offset, 6, TC_F);
    BOOST_CHECK(!q.second);
}

BOOST_AUTO_TEST_SUITE_END()
