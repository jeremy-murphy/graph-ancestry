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
            
            template <typename Vertex, typename Graph>
            void finish_vertex(Vertex const &v, Graph const &)
            {
                *result++ = v;
            }
        };
        
        // Convenience function for making the visitor.
        template <typename Inserter>
        eulerian_path<Inserter> make_eulerian_path(Inserter inserter)
        {
            return eulerian_path<Inserter>(inserter);
        }
        
        
        /*
        template <typename Inserter>
        struct vertex_level : public boost::default_dfs_visitor
        {
            Inserter result;
            
            vertex_level(Inserter result) : result(result) {}
            
            
        };
        */
        
        // Hand-rolled DFS because I can't think of something better.
        template <typename Graph, typename Vertex, typename Inserter>
        void directed_treee_dfs_impl(Graph g, Vertex u, Inserter output, unsigned height)
        {
            typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
            // std::stack<>
            *output++ = std::make_pair(u, height);
            auto const foo = boost::out_edges(u, g);
            // auto const f = std::bind(directed_treee_dfs<Graph, Vertex, Inserter>, g, std::placeholders::_1, output, height + 1);
            std::for_each(foo.first, foo.second, [&](edge_descriptor const &e)
            {
                directed_treee_dfs(g, boost::target(e, g), output, height + 1);
            });
        }

        
        template <typename Graph, typename Inserter>
        void directed_treee_dfs(Graph g, Inserter output)
        {
            typedef typename boost::graph_traits<Graph>::vertex_iterator vertex_iterator;
            typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
            auto const foo = boost::vertices(g);
            if(foo.first != foo.second)
            {
                std::stack<vertex_descriptor> bar;
                bar.push(*foo.first);
                while(!bar.empty())
                {
                    auto const x = bar.top();
                    auto const baz = boost::out_edges(x);
                    std::for_each(baz.first, baz.second, [](std::size_t v)
                    {
                        
                    });
                    bar.pop();
                }
                directed_treee_dfs(g, *foo.first, output, 0);
            }
        }
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
        boost::depth_first_search(input, boost::visitor(detail::make_eulerian_path(std::back_inserter(E))));
        // detail::directed_treee_dfs(input, std::back_inserter(EL));
        auto const x = std::unique(std::begin(E), std::end(E));
        E.erase(x, std::end(E));
        E.clear();
    }
}

#endif
