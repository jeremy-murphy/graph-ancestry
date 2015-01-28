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
 * @file predicates.hpp
 * @brief Predicates on graphs.
 */

#ifndef PREDICATES_HPP
#define PREDICATES_HPP

#include <boost/graph/graph_concepts.hpp>

namespace graph_algorithms
{
    struct is_source
    {
        template <typename Vertex, typename Graph>
        bool operator()(Vertex u, Graph const &G) const
        {
            BOOST_CONCEPT_ASSERT((boost::BidirectionalGraphConcept<Graph>));
            return in_degree(u, G) == 0;
        }
    };
    
    
    struct is_sink
    {
        template <typename Vertex, typename Graph>
        bool operator()(Vertex u, Graph const &G) const
        {
            BOOST_CONCEPT_ASSERT((boost::IncidenceGraphConcept<Graph>));
            return out_degree(u, G) == 0;
        }
    };
}

#endif
