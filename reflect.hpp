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
 * @file reflect.hpp
 * @brief Algorithms to reflect a graph along some line of reflection.
 */

#ifndef REFLECT_HPP
#define REFLECT_HPP

#include "predicates.hpp"

#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>

#include <unordered_map>

namespace graph_algorithms
{
    template <typename Graph>
    struct reflection_builder : boost::default_bfs_visitor
    {
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        typedef typename boost::graph_traits<Graph>::vertices_size_type vertices_size_type;
        
        Graph *g;
        vertices_size_type offset;
        
        reflection_builder(Graph &g, vertices_size_type offset) : g(&g), offset(offset) {}
        
        void examine_edge(edge_descriptor e, Graph const &h) const
        {
            using namespace boost;
            add_edge(target(e, h) + offset, source(e, h) + offset, *g);
        }
    };
    


    /** @brief Transform a graph G by adding its reversed reflection at the sources.
     *  @ingroup graph_algorithms
     *  @param G    Input graph.
     *  @param F    G reflected through its source vertices.
     * 
     *  Transform G into F such that if G' is G with all the edges reversed,
     *  F is the graph that results from merging the sources of G with the sinks
     *  of G'.
     * 
     *  Time complexity: O(V⋅E)
     *  Space complexity: queue used by BFS.
     * 
     *  Requires: source vertices are the lowest-numbered vertices. I.e. for n
     *  source vertices, they are numbered 0 to n-1 in the graph.
     * 
     *  Unknowns: number of source vertices.
     */ 
    template <typename BidirectionalGraph, typename OutputGraph, typename ColourMap, typename Buffer>
    void reflect_through_sources(BidirectionalGraph const &G, OutputGraph &F, ColourMap colour, Buffer &q)
    {
        using namespace boost;

        BOOST_CONCEPT_ASSERT((BidirectionalGraphConcept<BidirectionalGraph>));
        BOOST_CONCEPT_ASSERT((VertexListGraphConcept<OutputGraph>));
        BOOST_CONCEPT_ASSERT((AdjacencyGraphConcept<OutputGraph>));
        
    }
    
    
    /** @brief Transform a graph G by adding its reversed reflection at the sources.
     *  @ingroup graph_algorithms
     *  @param G A Graph.
     * 
     *  Transform G into F such that if G' is G with all the edges reversed,
     *  F is the graph that results from merging the sources of G with the sinks
     *  of G'.
     * 
     *  Time complexity: O(V⋅E)
     *  Space complexity: queue used by BFS.
     * 
     *  Requires: source vertices are the lowest-numbered vertices. I.e. for n
     *  source vertices, they are numbered 0 to n-1 in the graph.
     * 
     *  Unknowns: number of source vertices.
     */ 
    template <typename BidirectionalGraph, typename ColourMap, typename Buffer, typename N>
    void reflect_through_sources(BidirectionalGraph &G, ColourMap colour, Buffer &q, N n_sources)
    {
        using namespace boost;
        
        typedef typename graph_traits<BidirectionalGraph>::vertex_descriptor vertex_descriptor;
        typedef typename graph_traits<BidirectionalGraph>::edge_descriptor edge_descriptor;
        
        BOOST_CONCEPT_ASSERT((BidirectionalGraphConcept<BidirectionalGraph>));
        
        assert(n_sources >= 0);

        auto const offset = num_vertices(G) - n_sources;
        auto const builder = reflection_builder<BidirectionalGraph>(G, offset);
        auto V = vertices(G);
        for (N i = 0; i < n_sources; i++) // O(V) = s
        {
            vertex_descriptor const u = *V.first++;
            auto const E_u = out_edges(u, G);
            std::for_each(E_u.first, E_u.second, [&](edge_descriptor e) // O(E)
            {
                auto const new_vertex = target(e, G) + offset;
                auto const tmp = add_edge(new_vertex, u, G);
                assert(tmp.second);
                if (in_degree(new_vertex, G) == 0)
                {
                    assert(q.empty());
                    breadth_first_visit(G, target(e, G), q, builder, colour);
                }
            });
        }
    }
    
    
    template <typename Graph>
    void reflect_through_sources(Graph &G)
    {
        using namespace std;
        using namespace std::placeholders;
        
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        
        auto const V = vertices(G);
        auto const source_last = find_if_not(V.first, V.second, std::bind(is_source(), _1, G));
        auto const n_sources = distance(V.first, source_last);
        unordered_map<vertex_descriptor, boost::default_color_type> vertex_color;
        boost::associative_property_map< decltype(vertex_color) > colour(vertex_color);
        for_each(V.first, V.second, [&](vertex_descriptor u){ put(colour, u, boost::white_color); });
        boost::queue<vertex_descriptor> q;
        reflect_through_sources(G, colour, q, n_sources);
    }
}

#endif
