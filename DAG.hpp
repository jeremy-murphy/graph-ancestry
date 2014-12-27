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
#include <boost/graph/breadth_first_search.hpp>

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
    
    
    template <typename Graph>
    struct CAE_builder : boost::default_bfs_visitor
    {
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
        
        Graph *g;
        vertices_size_type offset;
        
        CAE_builder(Graph &g, vertices_size_type offset) : g(&g), offset(offset) {}
        
        void examine_edge(edge_descriptor e, Graph const &h) const
        {
            using namespace boost;
            add_edge(target(e, h) + offset, source(e, h) + offset, *g);
        }
    };

    
    /** @brief Transform a graph G by adding its reversed reflection at the sources.
     *  @ingroup graph_algorithms
     *  @tparam Graph A directed Graph type.
     *  @param G A Graph.
     * 
     *  Time complexity: Θ(n)
     * 
     */ 
    template <typename Graph>
    void common_ancestor_existence_graph(Graph &G)
    {
        using namespace std;
        using namespace std::placeholders;
        using boost::target;
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        
        vector<vertex_descriptor> sources;
        auto const it = boost::vertices(G);
        copy_if(it.first, it.second, back_inserter(sources), bind(is_source(), _1, G));
        auto const offset = boost::num_vertices(G) - sources.size();
        auto const builder = CAE_builder<Graph>(G, offset);
        for_each(begin(sources), end(sources), [&](vertex_descriptor u)
        {
            auto const edges = boost::out_edges(u, G);
            for_each(edges.first, edges.second, [&](edge_descriptor e)
            {
                boost::add_edge(target(e, G) + offset, u, G);
                // NOTE: This could be made faster by marking the visited nodes 
                // and not revisiting them on subsequent calls to bfs.
                boost::breadth_first_search(G, target(e, G), boost::visitor(builder));
            });
        });
    }
}

#endif
