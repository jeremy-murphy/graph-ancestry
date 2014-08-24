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

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <vector>
#include <iterator>
#include <algorithm>

namespace LCA
{
    namespace detail
    {
        /// DFS visitor calculates Eulerian path of a graph.
        template <typename Inserter>
        struct eulerian_path : public boost::default_dfs_visitor
        {
            Inserter result;
            
            eulerian_path(Inserter result) : result(result) {}
            
            template <typename Edge, typename Graph>
            void tree_edge(Edge const &e, Graph const &g)
            {
                *result++ = boost::source(e, g);
                *result++ = boost::target(e, g);
            }
        };
        
        // Convenience function for making the visitor.
        template <typename Inserter>
        eulerian_path<Inserter> make_eulerian_path(Inserter inserter)
        {
            return eulerian_path<Inserter>(inserter);
        }
        
    }
    
    template <typename Graph, typename OGraph>
    // requires: VertexListGraph(Graph)
    //           OGraph is mutable
    void preprocess(Graph const &input, OGraph &output)
    {
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        std::vector<vertex_descriptor> E;
        std::vector<unsigned> L;
        E.reserve(boost::num_vertices(input));
        boost::depth_first_search(input, boost::visitor(detail::make_eulerian_path(std::back_inserter(E))));
        E.clear();
    }
}

#endif
