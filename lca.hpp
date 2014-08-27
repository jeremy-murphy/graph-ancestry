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
#include <stack>

namespace LCA
{
    namespace detail
    {
        /// DFS visitor calculates Eulerian path of a graph (after some post-processing).
        template <typename T, typename O>
        struct eulerian_path : public boost::default_dfs_visitor
        {
            T previous;
            O result;
            bool start;
            
            eulerian_path(O result) : result(result), start(true) {}
            
            template <typename Edge, typename Graph>
            void tree_edge(Edge const &e, Graph const &g)
            {
                if(start || boost::source(e, g) != previous)
                    *result++ = boost::source(e, g);
                start = false;
                *result++ = previous = boost::target(e, g);
            }
            
            template <typename Vertex, typename Graph>
            void finish_vertex(Vertex const &v, Graph const &)
            {
                if(v != previous)
                    *result++ = v;
            }
        };
        
        // Convenience function for making the visitor.
        template <typename T, typename O>
        eulerian_path<T, O> make_eulerian_path(O output)
        {
            return eulerian_path<T, O>(output);
        }
        
        
        /*
        template <typename Inserter>
        struct vertex_level : public boost::default_dfs_visitor
        {
            Inserter result;
            
            vertex_level(Inserter result) : result(result) {}
            
            
        };
        */
    }
    
    
    template <typename Graph, typename OGraph>
    // requires: VertexListGraph(Graph)
    //           OGraph is mutable
    void preprocess(Graph const &input, OGraph &output)
    {
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef std::pair<vertex_descriptor, std::size_t> vertex_height;
        std::vector<vertex_descriptor> E;
        std::vector<vertex_height> EL;
        E.reserve(boost::num_vertices(input));
        boost::depth_first_search(input, boost::visitor(detail::make_eulerian_path<vertex_descriptor>(std::back_inserter(E))));
    }
}

#endif
