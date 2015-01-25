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
#include <boost/graph/adjacency_list.hpp>

#include <set>
#include <stdexcept>

namespace graph_algorithms
{
    struct found_target : public std::exception {};
    
    // Graph-based visitor equivalent of std::find.
    template <typename Vertex>
    struct bfs_find : public boost::default_bfs_visitor
    {
        Vertex target;
        
        bfs_find(Vertex target) : target(target) {}
        
        template <typename Graph>
        void discover_vertex(Vertex const &v, Graph const &) const
        {
            if(target == v)
                throw found_target();
        }
    };

    
    template <typename Vertex>
    bfs_find<Vertex> make_bfs_find(Vertex u)
    {
        return bfs_find<Vertex>(u);
    }
    
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
            if(start || boost::source(e, g) != previous)
                *result++ = boost::source(e, g);
            start = false;
            *result++ = previous = boost::target(e, g);
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
    
    
    /// Outputs an edge from each cycle in a graph.
    template <typename Edge, typename OutputIterator>
    struct cycle_detector : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<OutputIterator, Edge>));
        
        std::set<Edge> predecessors;
        OutputIterator result;

        cycle_detector(OutputIterator result) : result(result) {}
        
        template <typename Graph>
        void back_edge(Edge const &E, Graph&)
        {
            if(predecessors.find(E) == std::end(predecessors))
                *result++ = E;
        }
        
        template <typename Graph>
        void tree_edge(Edge const &E, Graph &)
        {
            predecessors.insert(E);
        }
    };
    
    
    template <typename Edge, typename OutputIterator>
    cycle_detector<Edge, OutputIterator> make_cycle_detector(OutputIterator &&result)
    {
        return cycle_detector<Edge, OutputIterator>(std::forward<OutputIterator>(result));
    }
    

    template <typename Vertex, typename Graph, typename KeyValueMap, typename Value, typename Relation>
    bool prop_relation_wrapper(Vertex u, Graph const &, KeyValueMap m, Value x, Relation r)
    {
        return r(get(m, u), x);
    }
    
    
    template <typename BinaryPredicate>
    class stop_on_discover_vertex_if : public boost::default_bfs_visitor
    {
        BinaryPredicate p;
        
    public:
        stop_on_discover_vertex_if(BinaryPredicate p) : p(p) {}
        
        template <typename Graph, typename Vertex>
        void discover_vertex(Vertex u, Graph const &G)
        {
            if (p(u, G))
                throw found_target();
        }
    };
    
    
    template <typename Predicate>
    stop_on_discover_vertex_if<Predicate> make_stop_on_discover_vertex_if(Predicate p)
    {
        return stop_on_discover_vertex_if<Predicate>(p);
    }
}
