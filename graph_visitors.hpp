#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <boost/concept_check.hpp>
#include <boost/concept/assert.hpp>

#include <set>


namespace graph_algorithms
{
    /// DFS visitor calculates Eulerian path of a graph.
    template <typename Vertex, typename O>
    struct eulerian_path : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<O, Vertex>));

        Vertex previous;
        O result;
        bool start;
        
        eulerian_path(O result) : result(result), start(true) {}
        
        template <typename Edge, typename Graph>
        void tree_edge(Edge const &e, Graph const &g)
        {
            if(start || boost::source(e, g) != previous)
                *result++ = boost::source(e, g);
            start = false;
            *result++ = previous = boost::target(e, g);
        }
        
        template <typename Graph>
        void finish_vertex(Vertex const &v, Graph const &)
        {
            if(v != previous)
                *result++ = v;
        }
    };
    
    
    template <typename Vertex, typename O>
    eulerian_path<Vertex, O> make_eulerian_path(O output)
    {
        return eulerian_path<Vertex, O>(output);
    }
    
    
    template <typename O>
    struct vertex_depth : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<O, std::size_t>));

        O result;
        std::size_t depth;
        std::size_t previous;
        
        vertex_depth(O result) : result(result), depth(0), previous(1) {}
        
        template <typename Edge, typename Graph>
        void tree_edge(Edge const &, Graph const &)
        {
            if(depth != previous)
                *result++ = depth;
            ++depth;
            *result++ = previous = depth;
        }
        
        template <typename Vertex, typename Graph>
        void finish_vertex(Vertex const &, Graph const &)
        {
            if(depth != previous)
                *result++ = depth;
            --depth;
        }
    };
    
    
    template <typename O>
    vertex_depth<O> make_vertex_depth(O result)
    {
        return vertex_depth<O>(result);
    }
    
    
    template <typename Edge, typename O>
    struct cycle_detector : public boost::default_dfs_visitor
    {
        BOOST_CONCEPT_ASSERT((boost::OutputIterator<O, Edge>));
        
        std::set<Edge> predecessors;
        O result;

        cycle_detector(O result) : result(result) {}
        
        template <typename Graph>
        void back_edge(Edge const &E, Graph&)
        {
            if(predecessors.find(E) == std::end(predecessors))
                *result++ = E;
        }
        
        template <typename Graph>
        void tree_edge(Edge const &E, Graph &)
        {
            predecessors.insert(E);
        }
    };
    
    
    template <typename Edge, typename O>
    cycle_detector<Edge, O> make_cycle_detector(O &&result)
    {
        return cycle_detector<Edge, O>(std::forward<O>(result));
    }
}
