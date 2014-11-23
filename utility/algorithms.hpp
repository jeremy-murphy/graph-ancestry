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

#include <iterator>
#include <cassert>
#include <type_traits>

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
    char unsigned lower_log2(N n)
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
    
    
    template <typename N>
    inline 
    char unsigned upper_log2(N n)
    {
        BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));
        
        assert(n != 0);
        auto result = lower_log2(n);
        if (n & 1u && n != 1u)
            result++;
        return result;
    }
}

#endif
