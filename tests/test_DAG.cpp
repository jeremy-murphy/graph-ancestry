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

#include <iostream>
#include <locale>

#include <boost/graph/adjacency_list.hpp>

using namespace graph_algorithms;
using namespace std;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

BOOST_FIXTURE_TEST_CASE(test_CAE, Bender_2005_4<>)
{
#ifndef NDEBUG
    {
        ofstream output("Bender_2005_4.dot");
        boost::write_graphviz(output, g);
    }
#endif
    common_ancestor_existence_graph(g);
#ifndef NDEBUG
    {
        ofstream output("Bender_2005_4_F.dot");
        boost::write_graphviz(output, g);
    }
#endif
    Bender_2005_4_F h;
    BOOST_CHECK_EQUAL(boost::num_vertices(g), boost::num_vertices(h.g));
    BOOST_CHECK_EQUAL(boost::num_edges(g), boost::num_edges(h.g));
    // TODO: And now to check for actual isomorphism!
}
