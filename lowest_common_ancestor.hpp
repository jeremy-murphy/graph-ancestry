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

#include <boost/multi_array.hpp>

#include <boost/function_output_iterator.hpp>

#include <boost/range.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/indexed.hpp>

#include <algorithm>
#include <vector>

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
     *  @param tree
     *  @param Euler_tour_index Eulerian sequence of vertices.
     *  @param L Eulerian sequence of vertex depths.
     *  @param R Representative vertices.
     *  @param M Sparse table.
     *
     *  Time complexity: Θ(n lg n)
     *
     */
    template <typename Graph, typename VertexIterator, typename IntegerContainer,
              typename IndexMultiArray>
    // requires: Directed(Graph)
    void LCA_preprocess(Graph const &data, VertexIterator Euler_tour_index,
                        IntegerContainer &Euler_tour_level, IndexMultiArray &sparse_table)
    {
        using namespace boost;
        using namespace general;

        typedef typename graph_traits<Graph>::vertex_descriptor vertex_descriptor;

        BOOST_CONCEPT_ASSERT((VertexListGraphConcept<Graph>)); // This might be too strict, I can't recall.
        // BOOST_CONCEPT_ASSERT((RandomAccessContainer<VertexIterator>));
        BOOST_CONCEPT_ASSERT((RandomAccessContainer<IntegerContainer>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::MutableMultiArrayConcept<IndexMultiArray, 2>));

        // requires: data is a tree

        depth_first_search(data, visitor(make_eulerian_path<vertex_descriptor>(Euler_tour_index))); // Θ(n)
        depth_first_search(data, visitor(make_vertex_depth(std::back_inserter(Euler_tour_level)))); // Θ(n)
        RMQ_sparse_table(Euler_tour_level, sparse_table); // Θ(n lg n)
    }


    /** @brief Query the lowest common ancestor of two vertices.
     *  @param u First descendent vertex
     *  @param v Second descendent vertex
     *  @ingroup LCA_algorithms
     *
     *  Time complexity: Θ(1)
     *
     *  Note: First and second descendent vertices can be specified in either order.
     *  That is, lca_query(u, v, ...) == lca_query(v, u, ...).
     */
    // TODO: Does R have to be a container?  Could it be a unary function?
    template <typename Vertex, typename VertexIndexable, typename IntegerRange,
              typename IndexIndexable, typename IndexMultiArray>
    typename VertexIndexable::value_type
    LCA(Vertex u, Vertex v, VertexIndexable const &Euler_tour_index,
                          IntegerRange const &Euler_tour_level, IndexIndexable const &representative,
                          IndexMultiArray const &sparse_table)
    {
        // BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<RandomAccessVertexIterator>));
        // BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<VertexDepthContainer>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::ConstMultiArrayConcept<IndexMultiArray, 2>));
        typedef typename boost::range_value<IndexIndexable>::type Index;

        BOOST_ASSERT(u < representative.size());
        BOOST_ASSERT(v < representative.size());

        Index i = representative[u], j = representative[v];
        if (j < i)
            std::swap(i, j);
        Index const minimum = general::RMQ(i, j, Euler_tour_level, sparse_table); // Θ(1)
        return Euler_tour_index[minimum];
    }



    template <typename Graph>
    class lowest_common_ancestor
    {
    private:
      typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
      typedef boost::multi_array_types::extent_range extent_range;

      std::vector<vertex_descriptor> Euler_tour_index_;
      std::vector<std::size_t> Euler_tour_level_;
      std::vector<std::size_t> representative_;
      boost::multi_array<std::size_t, 2> sparse_table;

    public:
      lowest_common_ancestor(Graph const &tree)
      : Euler_tour_index_(num_vertices(tree) > 2 ? 2 * num_vertices(tree) - 2 : num_vertices(tree)),
        Euler_tour_level_(Euler_tour_index_.size()),
        representative_(num_vertices(tree)),
        sparse_table(general::sparse_table_extent(Euler_tour_index_.size()))
      {
        LCA_preprocess(tree, boost::make_function_output_iterator(
                     general::make_reflection(Euler_tour_index_.begin(),
                                              representative_.begin())),
                     Euler_tour_level_, sparse_table);
      }

      vertex_descriptor operator()(vertex_descriptor u, vertex_descriptor v) const
      {
        return lowest_common_ancestor_query(u, v, Euler_tour_index_, Euler_tour_level_,
                                            representative_, sparse_table);
      }

      std::vector<vertex_descriptor> const &Euler_tour_index() const
      { return Euler_tour_index_; }

      std::vector<std::size_t> const &Euler_tour_level() const
      { return Euler_tour_level_; }

      std::vector<std::size_t> const &representative() const
      { return representative_; }

    };
}

#endif

