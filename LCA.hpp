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
 * @file LCA.hpp
 * @brief Berkman and Vishkin’s lowest common ancestor (LCA) algorithm.
 * M.A. Bender et al., Lowest common ancestors in trees and directed acyclic 
 * graphs, J. Algorithms 57 (2005) 75-94.
 */

#ifndef LCA_HPP
#define LCA_HPP

#include "RMQ.hpp"
#include "transformers.hpp"
#include "graph_visitors.hpp"

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/multi_array/concept_checks.hpp>

#include <algorithm>
#include <tuple>

/*  A note about parameter types.
 * 
 *  I encourage and practice the use of input and output iterators and 
 *  frequently admonish those who pass and return vectors and sets, etc.
 *  Sometimes, however, you really do need access to a container in a way
 *  that iterators can't provide, such as mixing random-access with back 
 *  insertion.
 * 
 *  That is why several algorithms in this library take containers as input
 *  and output parameters: it seemed like the most reasonable way to do it.
 */

namespace graph_algorithms
{
    /** @brief Preprocess a directed acyclic graph for LCA querying.
     *  @ingroup graph_algorithms
     *  @tparam Graph A Graph that satisfies the VertexListGraph concept.
     *  @tparam OIndex Output iterator of container indices.
     *  @param L vertex 'level' (i.e. depth)
     *  @param R representative elements
     * 
     *  Time complexity: Θ(n lg n)
     * 
     *  The reason for the unconventional parameter names (T, E, etc) is to closely immitate the journal article.
     */ 
    template <typename Graph, typename VertexContainer, typename VertexDepthContainer, typename IndexOutput, typename IndexMultiArray>
    // requires: Directed(Graph)
    void LCA_preprocess(Graph const &T, VertexContainer &E, VertexDepthContainer &L, IndexOutput R, IndexMultiArray &M)
    {
        using namespace boost;
        using namespace general;
        
        typedef typename VertexContainer::iterator vertex_iterator;
        typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;
        typedef typename std::iterator_traits<vertex_iterator>::difference_type vertex_difference_type;
        typedef std::pair<vertex_descriptor, vertex_difference_type> vertex_index_pair;

        BOOST_CONCEPT_ASSERT((VertexListGraphConcept<Graph>)); // This might be too strict, I can't recall.
        BOOST_CONCEPT_ASSERT((RandomAccessContainer<VertexContainer>));
        BOOST_CONCEPT_ASSERT((RandomAccessContainer<VertexDepthContainer>));
        BOOST_CONCEPT_ASSERT((OutputIterator<IndexOutput, vertex_index_pair>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::MutableMultiArrayConcept<IndexMultiArray, 2>));
        
        // requires: acyclic(T)
        
        depth_first_search(T, visitor(make_eulerian_path<vertex_descriptor>(std::back_inserter(E)))); // Θ(n)
        depth_first_search(T, visitor(make_vertex_depth(std::back_inserter(L)))); // Θ(n)
        // The key realization here is that if R outputs to a map, insert does not replace, thereby providing the representative element.
        std::transform(std::begin(E), std::end(E), R, element_index<vertex_iterator>()); // Θ(n)
        RMQ_preprocess(L, M); // Θ(n lg n)
    }


    // Convenience function.
    template <typename Graph, typename Tuple>
    void LCA_preprocess(Graph const &T, Tuple &x)
    {
        // NOTE: Can use ADL for get()?
        LCA_preprocess(T, get<0>(x), get<1>(x), std::inserter(get<2>(x), std::end(get<2>(x))), get<3>(x));
    }
    
    
    /** @brief Query the lowest common ancestor of two vertices.
     *  @param u First descendent vertex
     *  @param u Second descendent vertex
     * 
     *  Time complexity: Θ(1)
     * 
     *  Note: First and second descendent vertices can be specified in either order.
     *  That is, lca_query(u, v, ...) == lca_query(v, u, ...).
     */
    // TODO: Does R have to be a container?  Could it be a unary function?
    template <typename Vertex, typename VertexContainer, typename VertexDepthContainer, typename IndexContainer, typename IndexMultiArray>
    typename VertexContainer::value_type LCA(Vertex u, Vertex v, VertexContainer const &E, VertexDepthContainer const &L, IndexContainer const &R, IndexMultiArray const &M)
    {
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexContainer>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexDepthContainer>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::ConstMultiArrayConcept<IndexMultiArray, 2>));
        
        auto i = R[u], j = R[v];
        if (j < i)
            std::swap(i, j);
        auto const minimum = general::RMQ(i, j, L, M); // Θ(1)
        return E[minimum];
    }
    
    
    // Convenience function.
    template <typename Vertex, typename Tuple>
    typename std::tuple_element<0, Tuple>::type::value_type
    LCA(Vertex u, Vertex v, Tuple const &x)
    {
        // NOTE: Can use ADL for get()?
        return LCA(u, v, get<0>(x), get<1>(x), get<2>(x), get<3>(x));
    }
}

#endif
