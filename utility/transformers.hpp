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
    template <typename I>
    class element_index
    {
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<I>::difference_type difference_type;
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<I>::value_type value_type;
        typedef BOOST_DEDUCED_TYPENAME std::iterator_traits<I>::reference reference;
        typedef std::pair<value_type, difference_type> T;
        
        difference_type index;
        
    public:
        element_index(difference_type start = 0) : index(start) {}
        
        T operator()(reference const &x)
        {
            return T(x, index++);
        }
    };
}
