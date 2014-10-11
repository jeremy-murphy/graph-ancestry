/*
 *    Copyright (C) 2014  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>
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

/**
 * @file lca.hpp
 * @brief Berkman and Vishkin’s lowest common ancestor (LCA) algorithm.
 * M.A. Bender et al., Lowest common ancestors in trees and directed acyclic 
 * graphs, J. Algorithms 57 (2005) 75-94.
 */

#ifndef LCA_HPP
#define LCA_HPP

#include "RMQ.hpp"
#include "algorithms.hpp"
#include "graph_visitors.hpp"

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>


namespace graph_algorithms
{
    /** @brief Preprocess a directed acyclic graph for LCA querying.
     *  @ingroup graph_algorithms
     *  @tparam Graph A Graph that satisfies the VertexListGraph concept.
     *  @tparam OIndex Output iterator of container indices.
     *  @param L vertex 'level' (i.e. depth)
     *  @param R representative elements
     */ 
    template <typename Graph, typename VertexContainer, typename VertexDepthContainer, typename OIndex, typename IndexContainer>
    // requires: Directed(Graph)
    void lca_preprocess(Graph const &input, VertexContainer &Eulerian_path, VertexDepthContainer &L, OIndex R, IndexContainer &sparse_table)
    {
        BOOST_CONCEPT_ASSERT((boost::VertexListGraphConcept<Graph>)); // This might be too strict, I can't recall.
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexContainer>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexDepthContainer>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<IndexContainer>));
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<OIndex, std::size_t>));
        
        // requires: acyclic(input)
        typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        
        boost::depth_first_search(input, boost::visitor(make_eulerian_path<vertex_descriptor>(std::back_inserter(Eulerian_path)))); // Θ(n)
        boost::depth_first_search(input, boost::visitor(make_vertex_depth(std::back_inserter(L)))); // Θ(n)
        general::representative_element(Eulerian_path, R); // Θ(n)
        general::preprocess_sparse_table(std::begin(L), std::end(L), sparse_table); // Θ(n lg n)
    }


    /** @brief Query the lowest common ancestor of two vertices.
     *  @param u First descendent vertex
     *  @param u Second descendent vertex
     */
    template <typename Vertex, typename VertexContainer, typename VertexDepthContainer, typename IteratorContainer, typename IndexContainer>
    typename VertexContainer::value_type lca_query(Vertex u, Vertex v, VertexContainer const &Eulerian_path, VertexDepthContainer const &L, IteratorContainer const &R, IndexContainer const &sparse_table)
    {
        BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<Vertex>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexContainer>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<IteratorContainer>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<IndexContainer>));
        
        auto const minimum = general::query_sparse_table(R[u], R[v], L.size(), sparse_table);
        return Eulerian_path[std::distance(std::begin(L), minimum)];
    }
}

#endif
