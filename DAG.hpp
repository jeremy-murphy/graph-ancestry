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
#include <unordered_map>

#include <boost/concept/assert.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_iterator.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/breadth_first_search.hpp>

#ifndef NDEBUG
#include <iostream>
#include <typeinfo>
#endif

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
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
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
     *  Transform G into F such that if G' is G with all the edges reversed,
     *  F is the graph that results from merging the sources of G with the sinks
     *  of G'.
     * 
     *  Time complexity: transitive-closure
     *  Space complexity: queue used by BFS.
     * 
     *  Requires: source vertices are the lowest-numbered vertices. I.e. for n
     *  source vertices, they are numbered 0 to n-1 in the graph.
     * 
     *  Unknowns: number of source vertices.
     */ 
    template <typename Graph, typename ColourMap, typename Buffer, typename N>
    void common_ancestor_existence_graph(Graph &G, ColourMap colour, Buffer &q, N n_sources)
    {
        using namespace std;
        using boost::target;
        
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        
        auto const offset = boost::num_vertices(G) - n_sources;
        auto const builder = CAE_builder<Graph>(G, offset);
        auto V = boost::vertices(G);
        for (N i = 0; i < n_sources; i++) // O(V) = s
        {
            vertex_descriptor const u = *V.first++;
            auto const E_u = boost::out_edges(u, G);
            for_each(E_u.first, E_u.second, [&](edge_descriptor e) // O(E)
            {
                auto const new_vertex = target(e, G) + offset;
                auto const tmp = boost::add_edge(new_vertex, u, G);
                assert(tmp.second);
                if (boost::in_degree(new_vertex, G) == 0)
                {
                    assert(q.empty());
                    boost::breadth_first_visit(G, target(e, G), q, builder, colour);
                }
            });
        }
    }
    
    
    template <typename Graph>
    void common_ancestor_existence_graph(Graph &G)
    {
        using namespace std;
        using namespace std::placeholders;

        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;

        auto const V = boost::vertices(G);
        auto const source_last = find_if_not(V.first, V.second, bind(is_source(), _1, G));
        auto const n_sources = std::distance(V.first, source_last);
        unordered_map<vertex_descriptor, boost::default_color_type> vertex_color;
        boost::associative_property_map< decltype(vertex_color) > colour(vertex_color);
        for_each(V.first, V.second, [&](vertex_descriptor u){ put(colour, u, boost::white_color); });
        boost::queue<vertex_descriptor> q;
        common_ancestor_existence_graph(G, colour, q, n_sources);
    }
}

#endif
