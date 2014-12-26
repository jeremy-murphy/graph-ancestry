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
 * @file DAG.hpp
 * @brief Operations on directed acyclic graphs (DAG).
 * M.A. Bender et al., Lowest common ancestors in trees and directed acyclic 
 * graphs, J. Algorithms 57 (2005) 75-94.
 */

#ifndef DAG_HPP
#define DAG_HPP

#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>

#include <boost/concept/assert.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>

namespace graph_algorithms
{
    struct is_source
    {
        template <typename Vertex, typename Graph>
        bool operator()(Vertex u, Graph const &G) const
        {
            BOOST_CONCEPT_ASSERT(( boost::BidirectionalGraphConcept<Graph> ));
            return boost::in_degree(u, G) == 0;
        }
    };


    struct is_sink
    {
        template <typename Vertex, typename Graph>
        bool operator()(Vertex u, Graph const &G) const
        {
            return boost::out_degree(u, G) == 0;
        }
    };
    
    
    template <typename Graph, typename OGraph>
    void common_ancestor_existence_graph(Graph const &G, OGraph &F)
    {
        using namespace std::placeholders;
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;

        std::vector<vertex_descriptor> sources;
        auto it = boost::vertices(G);
        std::copy_if(it.first, it.second, std::back_inserter(sources), std::bind(is_source(), _1, G));
    }
}

#endif
