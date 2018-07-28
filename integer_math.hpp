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
 * @file integer_math.hpp
 * @brief General algorithms.
 */

#ifndef INTEGER_MATH_HPP
#define INTEGER_MATH_HPP

#include <boost/assert.hpp>


namespace general
{
    template <typename Integer>
    inline
    long unsigned pow2(Integer n)
    {
        BOOST_ASSERT(n >= 0);

        return 1ul << n;
    }
    
    
    template <typename N>
    inline
    char lower_log2(N n)
    {
        BOOST_ASSERT(n > 0);
        char result = 0;
        
        while (n >>= 1u)
            result++;
        
        return result;
    }
    
    
    template <typename N>
    inline 
    char upper_log2(N n)
    {
        BOOST_ASSERT(n > 0);
        char result = lower_log2(n);
        if (n & 1u && n != 1u)
            result++;
        return result;
    }
}

#endif
