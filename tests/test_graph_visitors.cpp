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
 * @file test_graph_visitors.hpp
 * @brief Unit tests for graph visitors.  
 */

#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "graph_visitors.hpp"
#include "fixture.hpp"

#include <iostream>
#include <locale>

using namespace graph_algorithms;
using namespace std;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

typedef Bender_2005_2<boost::adjacency_list<>> test_graph_type;

BOOST_GLOBAL_FIXTURE(enable_locale);

BOOST_FIXTURE_TEST_SUITE(TEST_GRAPH_VISITORS, test_graph_type)

BOOST_AUTO_TEST_CASE(test_vertex_depth)
{
    vector<size_t> L;
    boost::depth_first_search(g, boost::visitor(make_vertex_depth(back_inserter(L))));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
}


BOOST_AUTO_TEST_CASE(test_eulerian_path)
{
    vector<vertex_descriptor> E;
    boost::depth_first_search(g, boost::visitor(make_eulerian_path<vertex_descriptor>(back_inserter(E))));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
}


BOOST_AUTO_TEST_CASE(test_cycle_detector)
{
    vector<edge_descriptor> output;
    auto vis = make_cycle_detector<edge_descriptor>(back_inserter(output));
    boost::depth_first_search(g, boost::visitor(vis));
    BOOST_CHECK(output.empty());
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_CASE(test_cycle_detector)
{
    boost::adjacency_list<> g;
    typedef typename boost::graph_traits<boost::adjacency_list<>>::edge_descriptor edge_descriptor;
    // Make a simple cycle.
    boost::add_edge(0, 1, g);
    boost::add_edge(1, 2, g);
    boost::add_edge(2, 0, g);
    vector<edge_descriptor> output;
    auto vis = make_cycle_detector<edge_descriptor>(back_inserter(output));
    boost::depth_first_search(g, boost::visitor(vis));
    // Not sure about the guarantees on order of traversal so don't check for anything more specific.
    BOOST_CHECK_EQUAL(output.size(), 1);
}
