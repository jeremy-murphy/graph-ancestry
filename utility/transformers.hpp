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
 * @file transformers.hpp
 * @brief General transformer function objects.
 */

#ifndef GRAPH_TRANSFORMERS_HPP
#define GRAPH_TRANSFORMERS_HPP

#include <boost/config.hpp>

#include <iterator>
#include <utility>

namespace general
{
    /**
     * @brief Convert element => (element, index).
     * @ingroup function_objects
     * @tparam I Input iterator.
     * 
     * Starting from an initial index value, successive calls to element_index 
     * will convert an element to an (element, index) pair such that index
     * uniquely identifies each element.
     */
    template <typename Iterator>
    class element_index
    {
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<Iterator>::difference_type difference_type;
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<Iterator>::value_type value_type;
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<Iterator>::reference reference;
        typedef std::pair<value_type, difference_type> ValueIndexPair;
        
        difference_type index;
        
    public:
        element_index(difference_type start = 0) : index(start) {}
        
        ValueIndexPair operator()(reference const &x)
        {
            return ValueIndexPair(x, index++);
        }
    };


    // TODO: A better name.
    template <typename IntegerIterator0, typename IntegerIterator1>
    struct reflection
    {
      typedef typename std::iterator_traits<IntegerIterator0>::value_type Integer;

      IntegerIterator0 a;
      IntegerIterator1 b;
      Integer i;

      reflection(IntegerIterator0 a, IntegerIterator1 b, Integer start = 0)
      : a(a), b(b), i(start) {}

      Integer operator()(Integer u)
      {
        a[i] = u;
        b[u] = i;
        return i++;
      }
    };

    template <typename IntegerIterator0, typename IntegerIterator1>
    reflection<IntegerIterator0, IntegerIterator1>
    make_reflection(IntegerIterator0 a, IntegerIterator1 b, int start = 0)
    {
      return reflection<IntegerIterator0, IntegerIterator1>(a, b, start);
    }
}

#endif
