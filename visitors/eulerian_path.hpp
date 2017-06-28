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

#ifndef EULERIAN_PATH
#define EULERIAN_PATH

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>

#include <boost/graph/depth_first_search.hpp>


namespace graph_algorithms
{

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

} // graph_ancestry

#endif
