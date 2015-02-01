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
#include <boost/graph/transitive_closure.hpp>

#include <unordered_map>

namespace graph_algorithms
{
    template <typename OutputGraph>
    struct reflection_builder : boost::default_bfs_visitor
    {
        typedef typename boost::graph_traits<OutputGraph>::vertex_descriptor vertex_descriptor;
        // typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        typedef typename boost::graph_traits<OutputGraph>::vertices_size_type vertices_size_type;
        
        OutputGraph *F;
        vertices_size_type offset;
        
        reflection_builder(OutputGraph &F, vertices_size_type offset) : F(&F), offset(offset) {}
        
        template <typename Edge, typename Graph>
        void examine_edge(Edge e, Graph const &G) const
        {
            using namespace boost;
            add_edge(target(e, G) + offset, source(e, G) + offset, *F);
        }
    };
    

    template <typename Graph, typename N>
    reflection_builder<Graph> make_reflection_builder(Graph &g, N offset)
    {
        return reflection_builder<Graph>(g, offset);
    }

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
    /*
    template <typename BidirectionalGraph, typename OutputGraph, typename ColourMap, typename Buffer, typename N>
    void reflect_through_sources(BidirectionalGraph const &G, OutputGraph &F, ColourMap colour, Buffer &q, N n_sources)
    {
        using namespace boost;

        BOOST_CONCEPT_ASSERT((BidirectionalGraphConcept<BidirectionalGraph>));
        BOOST_CONCEPT_ASSERT((VertexListGraphConcept<OutputGraph>));
        BOOST_CONCEPT_ASSERT((AdjacencyGraphConcept<OutputGraph>));
        
        copy_graph(G, F);
    }
    */
    
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
    template <typename BidirectionalGraph, typename MutableGraph, typename ColourMap, typename Buffer, typename N>
    typename boost::graph_traits<BidirectionalGraph>::vertices_size_type 
    reflect_through_sources(BidirectionalGraph const &G, MutableGraph &F, ColourMap colour, Buffer &q, N n_sources)
    {
        using namespace boost;
        
        typedef typename graph_traits<BidirectionalGraph>::vertex_descriptor vertex_descriptor;
        typedef typename graph_traits<BidirectionalGraph>::edge_descriptor edge_descriptor;
        
        BOOST_CONCEPT_ASSERT((BidirectionalGraphConcept<BidirectionalGraph>));
        BOOST_CONCEPT_ASSERT((MutableGraphConcept<MutableGraph>));
        
        assert(n_sources >= 0);

        auto const offset = num_vertices(G) - n_sources;
        auto const builder = make_reflection_builder(F, offset);
        auto V = vertices(G);
        for (N i = 0; i < n_sources; i++) // O(V) = s
        {
            vertex_descriptor const u = *V.first++;
            auto const E_u = out_edges(u, G);
            std::for_each(E_u.first, E_u.second, [&](edge_descriptor e) // O(E)
            {
                auto const new_vertex = target(e, G) + offset;
                auto const tmp = add_edge(new_vertex, u, F);
                assert(tmp.second);
                if (in_degree(new_vertex, F) == 0)
                {
                    assert(q.empty());
                    breadth_first_visit(G, target(e, G), q, builder, colour);
                }
            });
        }
        return offset;
    }
    
    
    template <typename Graph, typename MutableGraph>
    typename boost::graph_traits<Graph>::vertices_size_type
    reflect_through_sources(Graph const &G, MutableGraph &F)
    {
        using namespace std;
        using std::placeholders::_1;
        
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        
        auto const V = vertices(G);
        auto const source_last = find_if_not(V.first, V.second, std::bind(is_source(), _1, G));
        auto const n_sources = distance(V.first, source_last);
        unordered_map<vertex_descriptor, boost::default_color_type> vertex_color;
        boost::associative_property_map< decltype(vertex_color) > colour(vertex_color);
        for_each(V.first, V.second, [&](vertex_descriptor u){ put(colour, u, boost::white_color); });
        boost::queue<vertex_descriptor> q;
        if (static_cast<void const *>(&G) != static_cast<void const *>(&F))
        {
            auto const E = edges(G);
            std::for_each(E.first, E.second, [&](edge_descriptor e){ add_edge(source(e, G), target(e, G), F); });
        }
        return reflect_through_sources(G, F, colour, q, n_sources);
    }
    
    
    template <typename MutableGraph>
    typename boost::graph_traits<MutableGraph>::vertices_size_type
    reflect_through_sources(MutableGraph &G)
    {
        return reflect_through_sources(G, G);
    }
    
    /**
     *  Note: G is also modified.
     */
    template <typename Graph, typename MutableGraph>
    typename boost::graph_traits<Graph>::vertices_size_type
    create_common_ancestor_existence_tc(Graph &G, MutableGraph &F)
    {
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;

        auto const offset = reflect_through_sources(G);
        Graph tmp;
        boost::transitive_closure(G, tmp);
        auto const E = edges(tmp);
        std::for_each(E.first, E.second, [&](edge_descriptor e){ add_edge(source(e, tmp), target(e, tmp), F); });
        
        return offset;
    }
}

#endif
