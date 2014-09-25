/*
 *    Copyright (C) 2014  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>
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
 * @file lca.hpp
 * @brief Berkman and Vishkin’s lowest common ancestor (LCA) algorithm.
 * M.A. Bender et al., Lowest common ancestors in trees and directed acyclic 
 * graphs, J. Algorithms 57 (2005) 75-94.
 */

#ifndef LCA_HPP
#define LCA_HPP

#include "RMQ.hpp"
#include "algorithms.hpp"
#include "graph_visitors.hpp"


namespace graph_algorithms
{
    template <typename Graph, typename VertexSequence, typename LevelSequence, typename OIterator, typename IteratorSequence>
    // requires: VertexListGraph(Graph) && Directed(Graph)
    void lca_preprocess(Graph const &input, VertexSequence &E, LevelSequence &L, OIterator R, IteratorSequence &T)
    {
        // requires: acyclic(input)
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        
        boost::depth_first_search(input, boost::visitor(make_eulerian_path<vertex_descriptor>(std::back_inserter(E)))); // Θ(n)
        boost::depth_first_search(input, boost::visitor(make_vertex_depth(std::back_inserter(L)))); // Θ(n)
        general::representative_element(std::begin(E), std::end(E), R); // Θ(n)
        general::preprocess_sparse_table(std::begin(L), std::end(L), T); // Θ(n lg n)
    }


    template <typename N, typename LevelSequence, typename VertexSequence>
    typename VertexSequence::value_type lca_query(N u, N v, LevelSequence L, VertexSequence E)
    {
        auto const minimum = general::query_sparse_table(u, v, L.size(), L);
        return E[minimum];
    }
}

#endif
