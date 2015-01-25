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
 * @brief Algorithms for the directed acyclic graph (DAG).
 */

#ifndef DAG_HPP
#define DAG_HPP

#include "graph_visitors.hpp"

#include <algorithm>
#include <vector>
#include <iterator>
#include <functional>
#include <unordered_map>

#include <boost/concept/assert.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/reverse_graph.hpp>

#ifndef NDEBUG
#include <iostream>
#include <typeinfo>
#endif

namespace graph_algorithms
{
    /**
     *  @brief Finds the first vertex that has a common ancestor with u.
     *  @ingroup non_mutating_algorithms
     *  @param  G       Directed acyclic graph.
     *  @param  u       Vertex to find common ancestor with.
     *  @param  first   Beginning of vertices to search.
     *  @param  last    End of vertices to search.
     *  @param  target  Which vertices are ancestors of u.
     *  @param  searched    Which vertices have been searched for a common ancestor.
     *  @param  q       Additional (?) vertices to search.
     * 
     */
    template <typename IncidenceGraph, typename Vertex, typename VertexInputIterator, typename VertexColourMap, typename VertexQueue>
    VertexInputIterator find_common_ancestor_existence(IncidenceGraph const &G, Vertex u, VertexInputIterator first, VertexInputIterator last, VertexColourMap target, VertexColourMap searched, VertexQueue &q)
    {
        using namespace boost;
        using namespace std::placeholders;
        
        BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<IncidenceGraph>));
        BOOST_CONCEPT_ASSERT((InputIterator<VertexInputIterator>));
        
        auto const H = make_reverse_graph(G);
        typedef decltype(H) ReversedGraph;
        breadth_first_visit(H, u, q, default_bfs_visitor(), target); // Update target_map.

        // TODO: Make a decision about how best to do this.
        typedef std::equal_to<default_color_type> PropRelation;
        auto const r = PropRelation();
        auto const stop_if_black_vertex = make_stop_on_discover_vertex_if(std::bind(prop_relation_wrapper<Vertex, ReversedGraph, VertexColourMap, default_color_type, PropRelation>, _1, _2, target, black_color, r));
        // auto const stop_if_black_vertex = make_stop_on_discover_vertex_if([&](Vertex v, ReversedGraph const &){ return get(target, v) == black_color; });
        
        try
        {
            for (; first != last; ++first)
            {
                Vertex const v = *first;
                // BGL does not check source vertex colour, so we do.
                if (get(searched, v) != black_color)
                    breadth_first_visit(H, v, q, stop_if_black_vertex, searched);
            }
        }
        catch (found_target const &)
        {
            return first;
        }
        return last;
    }
    
    
    /**
     *  @brief Finds the first vertex that has a common ancestor with u.
     *  @ingroup non_mutating_algorithms
     *  @param  G       Directed acyclic graph.
     *  @param  u       Vertex to find common ancestor with.
     *  @param  first   Beginning of vertices to search.
     *  @param  last    End of vertices to search.
     */
    template <typename IncidenceGraph, typename Vertex, typename VertexInputIterator>
    VertexInputIterator find_common_ancestor_existence(IncidenceGraph const &G, Vertex u, VertexInputIterator first, VertexInputIterator last)
    {
        using namespace boost;

        BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<IncidenceGraph>));
        BOOST_CONCEPT_ASSERT((InputIterator<VertexInputIterator>));

        typedef typename graph_traits<IncidenceGraph>::vertex_descriptor vertex_descriptor;
        
        auto const V = vertices(G);
        std::unordered_map<vertex_descriptor, default_color_type> target_colour, search_colour;
        typedef associative_property_map<decltype(target_colour)> ColourPropertyMap;
        ColourPropertyMap target_map(target_colour), search_map(search_colour);
        std::for_each(V.first, V.second, [&](vertex_descriptor u)
        {
            put(target_map, u, white_color);
            put(search_map, u, white_color);
        });
        queue<vertex_descriptor> q;
        return find_common_ancestor_existence(G, u, first, last, target_map, search_map, q);
    }
}

#endif
