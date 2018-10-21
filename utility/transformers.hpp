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
    template <typename RandomAccessIterator>
    class inverter
    {
    public:
        typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
        typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;

        inverter(RandomAccessIterator target, value_type start = 0)
            : target(target), i(start) {}

        value_type operator()(difference_type value)
        {
            target[value] = i;
            return i++;
        }

    private:
        RandomAccessIterator target;
        value_type i;
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

      // This is the essential 'reflection' operation.
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
