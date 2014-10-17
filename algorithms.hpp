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
 * @file algorithms.hpp
 * @brief General algorithms.
 */

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP

#include <unordered_set>
#include <iterator>
#include <cassert>

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>


namespace general
{
    template <typename N>
    inline
    long unsigned pow2(N n)
    {
        BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));

        return 1ul << n;
    }
    
    
    template <typename N>
    inline
    char unsigned log2(N n)
    {
        BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));

        assert(n != 0);
        char unsigned result = 0;
        
        while(n >>= 1u)
        {
            result++;
        }
        
        return result;
    }
    
    /*  A note about parameter types.
     * 
     *  I encourage and practice the use of input and output iterators and 
     *  frequently admonish those who pass and return vectors and sets, etc.
     *  Sometimes, however, you really do need access to a container in a way
     *  that iterators can't provide, such as by random-access.
     * 
     *  That is why several algorithms in this library take containers as input
     *  and output parameters: it seems like the most reasonable way to do it.
     */
    
    
    /** @brief Store in reps the lowest index for each unique value in a.
     *  @tparam C0 A random-access container of type T.
     *  @tparam C1 A type that provides T& operator[](T const &).
     *  @param a Input data.
     *  @param reps The indices of the representatives.
     *  
     *  Time complexity: Θ(n)
     *  Space complexity: worst-case Θ(n), best-case Θ(1), average-case ?
     *  C1 is typically either a random-access container or an associative map.
     *  The choice of container for C1 is constrained by T: R[T()] must be valid.
     *  E.g.: if UnsignedInteger<T> then C1 is unconstrained, but if String<T>
     *  then C1 cannot be an array-like container.
     *  Constraints aside, the density of input values may also influence which
     *  container is more efficient.  In general, high density will be better in
     *  an array, low density better in a map.
     */
    template <typename C0, typename C1>
    void representative_element(C0 const &a, C1 &reps)
    {
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<C0>));
        
        // Requires: reps[j] = i is valid for all j ∈ a.
        std::unordered_set<std::size_t> seen;
        for(auto first = std::begin(a); first != std::end(a); ++first)
        {
            if(seen.find(*first) == std::end(seen))
            {
                reps[*first] = std::distance(std::begin(a), first);
                seen.insert(*first);
            }
        }
    }
}

#endif
