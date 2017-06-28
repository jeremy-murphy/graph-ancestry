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
#ifndef VISITORS_HPP
#define VISITORS_HPP

/* LCA visitor awkwardness.
 * 
 * Whilst Boost.Graph provides many useful methods on the visitor for pre-,
 * post- and in-order processing, the following visitors (written for LCA)
 * require the sequence of vertices visited during the search as though a
 * physical token were moving from vertex to vertex. The resulting sequence
 * visits each vertex 1 + num_children(u) times, for a total length with an 
 * upper bound of |V| + |E|.
 * 
 * Unfortunately, there is no "visit" method on the visitor that exactly
 * provides this behaviour, so it is simulated by calls to tree_edge and
 * finish_vertex with checks for duplicate vertices.
 */

#include "visitors/eulerian_path.hpp"
#include "visitors/vertex_depth.hpp"

#include <stdexcept>
// TODO: This stuff needs to be put somewhere.
namespace graph_algorithms
{

template <typename T>
struct found_something : public std::exception
{
    T thing;
    
    found_something(T x) : thing(x) {}
};



template <typename Vertex, typename Graph, typename KeyValueMap, typename Value, typename Relation>
bool prop_relation_wrapper(Vertex u, Graph const &, KeyValueMap m, Value x, Relation r)
{
    return r(get(m, u), x);
}


template <typename BinaryPredicate, typename Tag>
class stop_on_vertex : public boost::base_visitor<stop_on_vertex<BinaryPredicate, Tag>>
{
    BinaryPredicate p;
    
public:
    typedef Tag event_filter;
    
    stop_on_vertex(BinaryPredicate p) : p(p) {}
    
    template <typename Graph, typename Vertex>
    void operator()(Vertex u, Graph const &G) const
    {
        if (p(u, G))
            throw found_something<Vertex>(u);
    }
};


template <typename BinaryPredicate, typename Tag>
stop_on_vertex<BinaryPredicate, Tag> make_stop_on_vertex(BinaryPredicate p, Tag)
{
    return stop_on_vertex<BinaryPredicate, Tag>(p);
}

}

#endif
