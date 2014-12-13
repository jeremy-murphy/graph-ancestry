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

#include <boost/concept_check.hpp>
#include <boost/concept/requires.hpp>
#include <boost/concept/assert.hpp>
#include <boost/multi_array/concept_checks.hpp>

#include <iterator>
#include <algorithm>
#include <cassert>
#include <cmath>

#include "algorithms.hpp"


namespace general
{
    /**
     * @defgroup RMQ_algorithms Range minimum query
     * @ingroup non_mutating_algorithms
     */
    
    
    /**
     * @brief           Create a Sparse Table of indexes for RMQ from the input container A.
     * @ingroup         RMQ_algorithms
     * @tparam C0       Read-only random-access container.
     * @tparam C1       Mutable 2d array.
     * @param A         Input array.
     * @param M         Sparse Table of A.
     */
    template <typename C0, typename C1>
    void preprocess_sparse_table(C0 const &A, C1 &M)
    {
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<C0>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::MutableMultiArrayConcept<C1, 2>));
        
        if(A.size() > 2)
        {
            typedef typename C0::size_type size_type;
            char unsigned j = 1;
            
            for(size_type i = 0; i < A.size() - 1; i++)
                M[j][i] = A[i] <= A[i + 1] ? i : i + 1;
            
            auto const n = A.size();
            auto const lowerlogn = lower_log2(n);
            auto prev_block_length = 2u;
            
            for(j = 2; j <= lowerlogn; j++)
            {
                auto const block_length = pow2(j);
                auto const last_pos = n - block_length + 1u;
                
                for(std::size_t i = 0; i != last_pos; i++)
                {
                    auto const &M1 = M[j - 1u][i], 
                                &M2 = M[j - 1u][i + prev_block_length];
                    M[j][i] = A[M2] < A[M1] ? M2 : M1;
                }
                prev_block_length = block_length;
            }
        }
    }

    
    /** @brief Perform range minimum query on a Sparse Table.
     *  @ingroup RMQ_algorithms
     *  @tparam N0 Index type
     *  @tparam N1 Container size type (usually std::size_t)
     *  @tparam C Container type
     *  @param i Lower bound index of range query
     *  @param j Upper bound index of range query
     *  @param n Size of original input problem
     *  @param sparse_table The Sparse Table to be queried
     * 
     *  Time complexity: Θ(1)
     */
    template <typename N, typename C0, typename C1>
    typename C1::element query_sparse_table(N i, N j, C0 const &A, C1 const &M)
    {
        BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<C0>));
        BOOST_CONCEPT_ASSERT((boost::multi_array_concepts::ConstMultiArrayConcept<C1, 2>));
        
        // requires: [i, j] is a valid range.
        assert(i <= j);
        
        if (i == j)
            return i;
        
        auto const r = j - i + 1;
        auto const k = lower_log2(r);
        auto const x = M[k][i], y = M[k][j + 1 - 2 * k];
        auto const &Mx = A[x], &My = A[y];
        return My < Mx ? y : x;
    }
}

#endif
