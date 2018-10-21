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
 * @file fixture.hpp
 * @brief Graph fixtures from Bender for unit tests.
 */

#ifndef FIXTURE_HPP
#define FIXTURE_HPP

#include <boost/assign.hpp>

#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/array.hpp>

#include <boost/unordered_map.hpp>

#include <iterator>
#include <vector>
#include <unordered_map>

#ifndef NDEBUG
#include <iostream>
#include <typeinfo>
#endif

typedef std::vector<std::size_t> index_vector;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS> DefaultAdjacencyList;

/// Figure 2 from the paper cited in LCA.hpp.
template <typename Graph = DefaultAdjacencyList>
struct Bender_2005_2
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef index_vector::const_iterator const_iterator;

    Graph g;
    boost::array<vertex_descriptor, 39> E;
    boost::array<std::size_t, 39> L;
    boost::array<std::size_t, 20> R_indices;
    boost::unordered_map<std::size_t, std::size_t> R = {{0, 0}, {1, 1}, {2, 21}, {3, 27}, {4, 2}, {5, 16}, {6, 22}, {7, 28}, {8, 36}, {9, 3}, {10, 5}, {11, 11}, {12, 17}, {13, 23}, {14, 29}, {15, 31}, {16, 33}, {17, 6}, {18, 8}, {19, 12}};
    
    // Manually entering iterators is tedious and they are confusing to read.
    std::vector<std::size_t> T_values = {0, 1, 2, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3, 2, 1, 1, 2, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    Bender_2005_2()
    : E(boost::assign::list_of(0)( 1)( 4)( 9)(4)(10)(17)(10)(18)(10)
                              (4)(11)(19)(11)(4)( 1)( 5)(12)( 5)( 1)
                              (0)( 2)( 6)(13)(6)( 2)( 0)( 3)( 7)(14)
                              (7)(15)( 7)(16)(7)( 3)( 8)( 3)( 0)),
    L(boost::assign::list_of(0)(1)(2)(3)(2)(3)(4)(3)(4)(3)
                            (2)(3)(4)(3)(2)(1)(2)(3)(2)(1)
                            (0)(1)(2)(3)(2)(1)(0)(1)(2)(3)
                            (2)(3)(2)(3)(2)(1)(2)(1)(0)),
    R_indices(boost::assign::list_of(0)( 1)(21)(27)( 2)(16)(22)(28)(36)( 3)
                                    (5)(11)(17)(23)(29)(31)(33)( 6)( 8)(12))
    {
        using boost::add_edge;
        for (int i = 0; i != 19; ++i)
            boost::add_vertex(g);

        add_edge(0, 1, g);
        add_edge(0, 2, g);
        add_edge(0, 3, g);
        add_edge(1, 4, g);
        add_edge(1, 5, g);
        add_edge(2, 6, g);
        add_edge(3, 7, g);
        add_edge(3, 8, g);
        add_edge(4, 9, g);
        add_edge(4, 10, g);
        add_edge(4, 11, g);
        add_edge(5, 12, g);
        add_edge(6, 13, g);
        add_edge(7, 14, g);
        add_edge(7, 15, g);
        add_edge(7, 16, g);
        add_edge(10, 17, g);
        add_edge(10, 18, g);
        add_edge(11, 19, g);
    }
};


template <typename Graph>
void add_Bender_2005_4(Graph &g)
{
    add_edge(0, 5, g);
    add_edge(1, 6, g);
    add_edge(5, 8, g);
    add_edge(6, 8, g);
    add_edge(2, 9, g);
    add_edge(2, 5, g);
    add_edge(3, 7, g);
    add_edge(4, 7, g);
    add_edge(7, 6, g);
    add_edge(6, 9, g);
    add_edge(7, 10, g);
    add_edge(10, 9, g);
}


template <typename Graph>
void add_Bender_2005_4_F(Graph &g)
{
    add_Bender_2005_4(g);
    add_edge(11, 0, g);
    add_edge(11, 2, g);
    add_edge(12, 1, g);
    add_edge(12, 13, g);
    add_edge(13, 3, g);
    add_edge(13, 4, g);
    add_edge(14, 11, g);
    add_edge(14, 12, g);
    add_edge(15, 2, g);
    add_edge(15, 12, g);
    add_edge(15, 16, g);
    add_edge(16, 13, g);
}


template <typename Graph = DefaultAdjacencyList>
struct Bender_2005_4
{
    Graph g;
    
    Bender_2005_4()
    {
        add_Bender_2005_4(g);
    }
};

template <typename Graph = DefaultAdjacencyList>
struct Bender_2005_4_F
{
    Graph g;
    
    Bender_2005_4_F()
    {
        add_Bender_2005_4_F(g);
    }
};

#endif
