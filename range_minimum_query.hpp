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
 * @file RMQ.hpp
 * @brief Range minimum query.
 */

#ifndef RMQ_HPP
#define RMQ_HPP

// #include <boost/concept_check.hpp>
// #include <boost/concept/requires.hpp>
// #include <boost/concept/assert.hpp>

#include <integer_math.hpp>

#include <boost/config.hpp>

// #include <boost/multi_array/concept_checks.hpp>
#include <boost/multi_array.hpp>

#include <boost/range.hpp>

#include <iterator>



namespace general
{
    /**
     * @defgroup RMQ_algorithms Range minimum query
     * @ingroup non_mutating_algorithms
     */
    
    
    /**
     * @brief               Create a Sparse Table of indexes for RMQ from the input container A.
     * @ingroup             RMQ_algorithms
     * @param range          Input range.
     * @param sparse_table  Metadata created to process queries on the range.
     *
     * Time complexity: O(n lg n)
     */
    template <typename Iterator, typename MultiArray>
    void RMQ_sparse_table(Iterator first,
                          typename std::iterator_traits<Iterator>::difference_type n,
                          MultiArray &sparse_table)
    {
        // BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<RandomAccessRange>));
        // BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::MutableMultiArrayConcept<MultiArray, 2>));
        

        typedef typename std::iterator_traits<Iterator>::difference_type difference_type;
        typedef typename MultiArray::element element;

        BOOST_ASSERT(n >= 0);

        if (n > 2)
        {
            BOOST_ASSERT(sparse_table.num_elements() >= n * lower_log2(n));

            for (difference_type i = 0; i != n - 1; i++)
                sparse_table[0][i] = first[i] <= first[i + 1] ? i : i + 1;

            char const lowerlogn = lower_log2(n);
            difference_type prev_block_length = 2;
            
            for (char j = 2; j != lowerlogn + 1; j++)
            {
                difference_type const block_length = pow2(j);
                difference_type const last_pos = n - block_length + 1;
                
                for (difference_type i = 0; i != last_pos; i++)
                {
                    element const &M1 = sparse_table[j - 2][i],
                                  &M2 = sparse_table[j - 2][i + prev_block_length];
                    sparse_table[j - 1][i] = first[M2] < first[M1] ? M2 : M1;
                }
                prev_block_length = block_length;
            }
        }
        // TODO: Do something for n = 2?
    }


    template <typename Iterator, typename MultiArray>
    void RMQ_sparse_table(Iterator first, Iterator last, MultiArray &sparse_table)
    {
      return RMQ_sparse_table(first, last - first, sparse_table);
    }

    template <typename RandomAccessRange, typename MultiArray>
    void RMQ_sparse_table(RandomAccessRange const &range, MultiArray &sparse_table)
    {
        return RMQ_sparse_table(boost::begin(range), boost::end(range), sparse_table);
    }


    /** @brief Perform range minimum query on a Sparse Table.
     *  @ingroup RMQ_algorithms
     *  @tparam N0 Index type
     *  @tparam N1 Container size type
     *  @tparam C Container type
     *  @param i Lower bound index of range query
     *  @param j Upper bound index of range query
     *  @param range
     *  @param sparse_table The Sparse Table to be queried
     * 
     *  Time complexity: Θ(1)
     */
    template <typename N, typename RandomAccessRange, typename MultiArray>
    typename MultiArray::element
    RMQ(N i, N j, RandomAccessRange const &range, MultiArray const &sparse_table)
    {
        // BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<RandomAccessRange>));
        // BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::ConstMultiArrayConcept<MultiArray, 2>));

        typedef typename MultiArray::element element;

        // requires: [i, j] is a valid range.
        BOOST_ASSERT(i >= 0);
        BOOST_ASSERT(i <= j);
        BOOST_ASSERT(i < boost::size(range));
        
        if (i == j)
            return i;
        
        N const r = j - i + 1;
        char const k = lower_log2(r);
        element const x = sparse_table[k - 1][i],
                      y = sparse_table[k - 1][j - pow2(k) + 1];
        BOOST_ASSERT(x >= i && x <= j);
        BOOST_ASSERT(y >= i && y <= j);
        return range[y] < range[x] ? y : x;
    }

    template <typename Integer>
    boost::multi_array_types::extent_gen::gen_type<2>::type sparse_table_extent(Integer n)
    {
        return boost::extents[n == 0 ? 0 : lower_log2(n)][n];
    }


    template <typename RandomAccessRange>
    class range_minimum_query
    {
      typedef typename boost::range_size<RandomAccessRange>::type index_t;
    public:
        range_minimum_query(RandomAccessRange const &range)
            : range(range),
              sparse_table(sparse_table_extent(boost::size(range)))
        {
            RMQ_sparse_table(range, sparse_table);
        }

        template <typename N>
        index_t operator()(N i, N j) const
        {
            return RMQ(i, j, range, sparse_table);
        }

    private:
        RandomAccessRange range;
        boost::multi_array<index_t, 2> sparse_table;
    };

    template <typename RandomAccessRange>
    range_minimum_query<RandomAccessRange>
    make_range_minimum_query(RandomAccessRange const &range)
    {
      return range_minimum_query<RandomAccessRange>(range);
    }
}

#endif
