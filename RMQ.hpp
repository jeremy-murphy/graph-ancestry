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

#include <iterator>
#include <algorithm>
#include <cassert>


namespace RMQ
{
    template <typename N>
    // requires: UnsignedIntegral(N)
    inline
    unsigned long pow2(N n)
    {
        return 1ul << n;
    }
    
    
    // Maybe just for my own sanity, but there needs to be a transformation from the
    // M(i, j) syntax to the M(ij) syntax.
    // e.g.:    (1, 1) = 0, (2, 1) = 1, (3, 1) = 2
    //          (1, 2) = (n * (j - 1) - ∑n - 2^j)
    
    // ∑2^j + k = 
    template <typename N0, typename N1, typename N2>
    // requires: UnsignedIntegral(N0) && UnsignedIntegral(N1) && UnsignedIntegral(N2)
    inline
    auto translate(N0 i, N1 j, N2 n) -> decltype(j * n + i)
    {
        return j == 1 ? i : ((j - N1(1)) * n) - (pow2(j) - j - N1(1)) + i;
    }
    
    
    template <typename I, typename OSequence>
    // requires: ForwardIterator(I)
    //           RandomAccessSequence(OSequence)
    void preprocess_sparse_table(I first, I last, OSequence &M)
    {
        if(first != last && std::next(first) != last)
        {
            for(auto Ai = first, Aj = std::next(Ai); Aj != last; ++Ai, ++Aj)
                M.push_back(*Ai <= *Aj ? Ai : Aj);
            
            auto const n = M.size() + 1;
            char unsigned const lowerlogn = std::log2(n);
            for(char unsigned j = 2; j <= lowerlogn; j++)
            {
                auto const block_length = pow2(j), block_length_2 = block_length / 2;
                auto const offset = n - block_length_2 + 1;
                std::size_t Mi = M.size() - offset; // Use index because of iterator invalidation.

                for(std::size_t i = 0; i != n - block_length + 1; ++i)
                {
                    auto const &M1 = M[Mi], &M2 = M[Mi + block_length_2];
                    M.push_back(*M1 <= *M2 ? M1 : M2);
                    ++Mi;
                }
            }
        }
    }
    
    
    template <typename N0, typename N1, typename N2, typename Sequence>
    // requires: UnsignedIntegral(N0..N2)
    typename Sequence::value_type query_sparse_table(N0 i, N1 j, N2 n, Sequence M)
    {
        // requires: [i, j] is a valid range.
        assert(i <= j);
        assert(j < n);
        if(i == j)
            return M[i];
        char unsigned const k = std::log2(j - i + 1);
        auto const block_size = pow2(k);
        auto const x = translate(i, k, n), 
                   y = translate(i + (j - i) - block_size + N0(1), k, n);
        auto const &Mx = M[x], &My = M[y];
        return *My < *Mx ? My : Mx;
    }
    
    
    template <typename I>
    // requires: ForwardIterator(I)
    class sparse_table
    {
        std::vector<I> M;
        
    public:
        sparse_table(I first, I last)
        {
            preprocess_sparse_table(first, last, M);
        }
        
        
        void preprocess(I first, I last)
        {
            M.clear();
            preprocess_sparse_table(first, last, M);
        }
        
        
        template <typename N>
        I query(N i, N j)
        {
            return query_sparse_table(i, j, M);
        }
    };
}

#endif
