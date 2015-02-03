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
 * @file graph_visitors.hpp
 * @brief Boost.Graph visitors for depth-first or breadth-first search.
 */
#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/breadth_first_search.hpp>

#include <set>
#include <stdexcept>

namespace graph_algorithms
{
    template <typename T>
    struct found_something : public std::exception
    {
        T thing;
        
        found_something(T x) : thing(x) {}
    };
    
    
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

    /// DFS visitor calculates Eulerian path of a graph.
    template <typename Vertex, typename O>
    struct eulerian_path : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<O, Vertex>));

        Vertex previous;
        O result;
        bool start;
        
        eulerian_path(O result) : result(result), start(true) {}
        
        // See the comment above for the reason why this is complicated.
        template <typename Edge, typename Graph>
        void tree_edge(Edge const &e, Graph const &g)
        {
            if(start || source(e, g) != previous)
                *result++ = source(e, g);
            start = false;
            *result++ = previous = target(e, g);
        }
        
        template <typename Graph>
        void finish_vertex(Vertex const &v, Graph const &)
        {
            if(v != previous)
                *result++ = v;
        }
    };
    
    
    template <typename Vertex, typename O>
    eulerian_path<Vertex, O> make_eulerian_path(O output)
    {
        return eulerian_path<Vertex, O>(output);
    }
    
    
    /// Outputs depth of each node it encounters given an arbitrary tree root on the graph.
    template <typename O>
    struct vertex_depth : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<O, std::size_t>));

        O result;
        std::size_t depth;
        std::size_t previous;
        
        vertex_depth(O result) : result(result), depth(0), previous(1) {}
        
        // See the comment above for the reason why this is complicated.
        template <typename Edge, typename Graph>
        void tree_edge(Edge const &, Graph const &)
        {
            if(depth != previous)
                *result++ = depth;
            ++depth;
            *result++ = previous = depth;
        }
        
        template <typename Vertex, typename Graph>
        void finish_vertex(Vertex const &, Graph const &)
        {
            if(depth != previous)
                *result++ = depth;
            --depth;
        }
    };
    
    
    template <typename O>
    vertex_depth<O> make_vertex_depth(O result)
    {
        return vertex_depth<O>(result);
    }
    

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
