/*
 *    Copyright (C) 2017  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>
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

#ifndef VERTEX_DEPTH
#define VERTEX_DEPTH

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>

#include <boost/graph/depth_first_search.hpp>


namespace graph_algorithms
{

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

    
} // graph_ancestry

#endif
