/*
 *    Copyright (C) 2015  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file test_reflect.hpp
 * @brief Unit tests for reflection functions.  
 */

#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "common_ancestor_existence.hpp"
#include "fixture.hpp"

#include <boost/graph/adjacency_matrix.hpp>

#include <iostream>
#include <locale>

using namespace graph_algorithms;

struct enable_locale
{
    enable_locale() { std::cout.imbue(std::locale("")); std::cerr.imbue(std::locale("")); }
};

BOOST_FIXTURE_TEST_SUITE(common_ancestor_existence, Bender_2005_4<DefaultAdjacencyList>)

BOOST_AUTO_TEST_CASE(basic)
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
