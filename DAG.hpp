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
     *  @brief Finds the first vertex that has a common ancestor 
     *  @ingroup non_mutating_algorithms
     *  @param  G       Directed acyclic graph.
     *  @param  u       Vertex to find common ancestor with.
     *  @param  first   Beginning of vertices to search.
     *  @param  last    End of vertices to search.
     *  @param  target  Which vertices are ancestors of u.
     *  @param  searched    Which vertices have been searched for a common ancestor.
     *  @param  q       Additional (?) vertices to search.
     *  @param  predecessor Vertex predecessors seen during search.
     * 
     */
    template <typename IncidenceGraph, typename VertexInputIterator, typename VertexColourMap, typename VertexQueue, typename PredecessorMap>
    VertexInputIterator find_common_ancestor_existence_impl(IncidenceGraph const &H, VertexInputIterator first, VertexInputIterator last, VertexColourMap const ancestors, VertexColourMap searched, VertexQueue &q, PredecessorMap predecessor)
    {
        using namespace boost;
        using namespace std::placeholders;
        
        typedef typename graph_traits<IncidenceGraph>::vertex_descriptor vertex_descriptor;

        BOOST_CONCEPT_ASSERT((IncidenceGraphConcept<IncidenceGraph>));
        BOOST_CONCEPT_ASSERT((InputIterator<VertexInputIterator>));
        
        // TODO: Make a decision about how best to do this.
        typedef std::equal_to<default_color_type> PropRelation;
        auto const r = PropRelation();
        auto const stop_if_black_vertex = make_stop_on_vertex(std::bind(prop_relation_wrapper<vertex_descriptor, IncidenceGraph, VertexColourMap, default_color_type, PropRelation>, _1, _2, ancestors, black_color, r), on_discover_vertex());
        // auto const stop_if_black_vertex = make_stop_on_discover_vertex_if([&](vertex_descriptor v, ReversedGraph const &){ return get(target, v) == black_color; });
        auto const foo = make_bfs_visitor(std::make_pair(stop_if_black_vertex, record_predecessors(make_assoc_property_map(predecessor), boost::on_tree_edge())));
        
        vertex_descriptor v;
        try
        {
            for (; first != last; ++first)
            {
                v = *first;
                // BGL does not check source vertex colour, so we do.
                if (get(searched, v) != black_color)
                {
                    breadth_first_visit(H, v, q, foo, searched);
                    predecessor.clear();
                }
            }
        }
        catch (found_something<vertex_descriptor> const &e)
        {
            // Mark predecessors in searched white.
            typedef typename PredecessorMap::value_type value_type;
            std::for_each(std::begin(predecessor), std::end(predecessor), [&](value_type const &x)
            {
                put(searched, x.first, white_color);
            });
            put(searched, v, white_color);
            
            for (auto u = e.thing; u != v; )
            {
                u = predecessor[u];
                put(ancestors, u, black_color);
            }
            
            predecessor.clear();
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
        vector_property_map<default_color_type> ancestors, searched;
        std::for_each(V.first, V.second, [&](vertex_descriptor u)
        {
            put(ancestors, u, white_color);
            put(searched, u, white_color);
        });
        queue<vertex_descriptor> q;
        std::unordered_map<vertex_descriptor, vertex_descriptor> predecessor;
        
        auto const H = make_reverse_graph(G);
        breadth_first_visit(H, u, q, default_bfs_visitor(), ancestors);
        
        return find_common_ancestor_existence_impl(H, first, last, ancestors, searched, q, predecessor);
    }
}

#endif
