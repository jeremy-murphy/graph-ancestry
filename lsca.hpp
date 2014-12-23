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
 * @file lsca.hpp
 * @brief Lowest single common ancestor (LSCA).
 * J. Fischer, D.H. Huson, New common ancestor problems in trees and directed 
 * acyclic graphs, Inf. Proc. Lett. 110 (2010) 331-335.
 */

#ifndef LSCA_HPP
#define LSCA_HPP

#include <boost/graph/topological_sort.hpp>
#include <boost/graph/graph_concepts.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <vector>
#include <iterator>
#include <algorithm>

namespace LSCA
{
    template <typename Graph, typename OGraph>
    // requires: VertexListGraph(Graph)
    //           OGraph is mutable
    void preprocess_lsca(Graph const &G, OGraph &output)
    {
        // requires: input is a dag.
        //           output is empty.
        /*
         * 1. In a depth-first traversal of G, compute a topological sort L of G.
         * 2. Initialize T_G to be the empty tree. T_G will be stored using the 
         * dynamic data structure for O(1)-LCAs from Fact 1.
         * 3. For all v ∈ V in the order of L: compute
         *      l = lca_T_G(parents(v))
         * as the LSA of v, and add v to T G as the child of l.
         */
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;

        BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Graph>)); // NOTE: This might be too strict.
        BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<OGraph>)); // NOTE: This might be too strict.
        
        auto const n = boost::num_vertices(G);
        auto const m = boost::num_edges(G);
        std::vector<vertex_descriptor> L;
        boost::topological_sort(G, std::back_inserter(L));
        std::reverse(std::begin(L), std::end(L));
        OGraph T_G;
        
    }
}





#endif
