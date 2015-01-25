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

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/isomorphism.hpp>

#include <iostream>
#include <locale>

using namespace graph_algorithms;
using namespace std;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};



BOOST_FIXTURE_TEST_CASE(test_find_common_ancestor_existence, Bender_2005_4<>)
{
    auto const V = boost::vertices(g);
    auto result = find_common_ancestor_existence(g, 0u, std::next(V.first, 6), V.second);
    BOOST_CHECK(result == std::next(V.first, 8));
    result = find_common_ancestor_existence(g, 5u, std::next(V.first, 6), V.second);
    BOOST_CHECK(result == std::next(V.first, 8));
    result = find_common_ancestor_existence(g, 5u, std::next(V.first, 3), V.second);
    BOOST_CHECK(result == std::next(V.first, 5));
    result = find_common_ancestor_existence(g, 1u, std::next(V.first, 2), V.second);
    BOOST_CHECK(result == std::next(V.first, 6));
    result = find_common_ancestor_existence(g, 8u, std::next(V.first, 0), V.second);
    BOOST_CHECK(result == std::next(V.first, 0));
    result = find_common_ancestor_existence(g, 8u, std::next(V.first, 5), V.second);
    BOOST_CHECK(result == std::next(V.first, 5));
}

