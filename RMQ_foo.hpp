/*
 *    Copyright (C) 2014  Jeremy W. Murphy <jeremy.william.murphy@gmail.com>
 * 
 *    This program is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 * 
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 * 
 *    You should have received a copy of the GNU General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @file RMQ_foo.hpp
 * @brief Range minimum query convenience class.
 */

#ifndef RMQ_FOO_HPP
#define RMQ_FOO_HPP

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>
#include <boost/core/ref.hpp>
#include <boost/multi_array.hpp>
#include <boost/operators.hpp>

#include "RMQ.hpp"
#include "algorithms.hpp"

namespace general
{
    template <typename C>
    class sparse_table : public boost::equality_comparable<sparse_table<C>>
    {
        BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<C>));
    public:
        typedef typename C::size_type value_type;
        typedef boost::multi_array<value_type, 2> array_type;
        
    private:
        typedef boost::multi_array_types::extent_range extent_range;
        C const *A;
        array_type M;
        
    public:
        sparse_table() {} // Partially formed.
        
        explicit
        sparse_table(C const &A) : A(&A), M(boost::extents[extent_range(1, lower_log2(A.size()) + 1)][A.size()])
        {
            preprocess_sparse_table(A, M);
        }
        
        void preprocess(C const &A)
        {
            this->A = &A;
            M = array_type(boost::extents[extent_range(1, lower_log2(A.size()) + 1)][A.size()]);
            preprocess_sparse_table(A, M);
        }
        
        template <typename N>
        value_type query(N i, N j) const
        {
            BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));
            return query_sparse_table(i, j, *A, M);
        }
        
        friend
        bool operator==(sparse_table const &x, sparse_table const &y)
        {
            return *x.A == *y.A && x.M == y.M;
        }
    };
    
    
    template <typename C>
    sparse_table<C> make_sparse_table(C const &A)
    {
        return sparse_table<C>(A);
    }
}

#endif
