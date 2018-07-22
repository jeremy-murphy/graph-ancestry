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

#include <range_minimum_query.hpp>
#include <transformers.hpp>
#include <visitors.hpp>

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/config.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/multi_array/concept_checks.hpp>

#include <boost/range.hpp>
#include <boost/range/algorithm/transform.hpp>

#include <algorithm>
#include <tuple>

namespace graph_algorithms
{
    /**
     * @defgroup LCA_algorithms Lowest common ancestor
     * @ingroup graph_algorithms
     */

    /** @brief Preprocess a directed acyclic graph for LCA querying.
     *  @ingroup LCA_algorithms
     *  @tparam Graph A Graph that satisfies the VertexListGraph concept.
     *  @tparam VertexContainer Random-access container of vertices.
     *  @tparam VertexDepthContainer Random-access container of vertex depth.
     *  @tparam IndexOutput Output iterator of container index.
     *  @tparam IndexMultiArray Mutable 2d array.
     *  @param T Tree.
     *  @param Euler_tour_index Eulerian sequence of vertices.
     *  @param L Eulerian sequence of vertex depths.
     *  @param R Representative vertices.
     *  @param M Sparse table.
     * 
     *  Time complexity: Θ(n lg n)
     * 
     *  The reason for the unconventional parameter names (T, E, etc) is to closely immitate the journal article upon which it is based.
     */ 
    template <typename Graph, typename VertexContainer, typename VertexDepthContainer, typename IndexOutput, typename IndexMultiArray>
    // requires: Directed(Graph)
    void LCA_preprocess(Graph const &tree, VertexContainer &Euler_tour_index,
                        VertexDepthContainer &Euler_tour_level, IndexOutput representative,
                        IndexMultiArray &sparse_table)
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
        
        // requires: acyclic(tree)
        
        depth_first_search(tree, visitor(make_eulerian_path<vertex_descriptor>(std::back_inserter(Euler_tour_index)))); // Θ(n)
        depth_first_search(tree, visitor(make_vertex_depth(std::back_inserter(Euler_tour_level)))); // Θ(n)
        // The key realization here is that if R outputs to a map, insert does not replace, thereby providing the representative element.
        std::transform(std::begin(Euler_tour_index), std::end(Euler_tour_index), representative, element_index<vertex_iterator>()); // Θ(n)
        RMQ_preprocess(Euler_tour_level, sparse_table); // Θ(n lg n)
    }


    // Convenience function.
    /// @ingroup LCA_algorithms
    template <typename Graph, typename Tuple>
    void LCA_preprocess(Graph const &T, Tuple &x)
    {
        // NOTE: Can use ADL for get()?
        LCA_preprocess(T, get<0>(x), get<1>(x), std::inserter(get<2>(x), std::end(get<2>(x))), get<3>(x));
    }


    /** @brief Query the lowest common ancestor of two vertices.
     *  @param u First descendent vertex
     *  @param u Second descendent vertex
     *  @ingroup LCA_algorithms
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


    /** Convenience function.
     *  @ingroup LCA_algorithms
     */
    template <typename Vertex, typename Tuple>
    typename std::tuple_element<0, Tuple>::type::value_type
    LCA(Vertex u, Vertex v, Tuple const &x)
    {
        // NOTE: Can use ADL for get()?
        return LCA(u, v, get<0>(x), get<1>(x), get<2>(x), get<3>(x));
    }
}

#endif
