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


namespace RMQ
{
    template <typename N>
    // requires: UnsignedIntegral(N)
    inline
    unsigned long pow2(N n)
    {
        return 1ul << n;
    }
    
    
    template <typename I, typename OSequence>
    // requires: ForwardIterator(I)
    void sparse_table(I first, I last, OSequence &M)
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
                    auto const M1 = M[Mi], M2 = M[Mi + block_length_2];
                    M.push_back(*M1 <= *M2 ? M1 : M2);
                    ++Mi;
                }
            }
        }
    }
}

#endif
