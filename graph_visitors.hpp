#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/adjacency_list.hpp>


namespace graph_algorithms
{
    /// DFS visitor calculates Eulerian path of a graph.
    template <typename T, typename O>
    struct eulerian_path : public boost::default_dfs_visitor
    {
        T previous;
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
        
        template <typename Vertex, typename Graph>
        void finish_vertex(Vertex const &v, Graph const &)
        {
            if(v != previous)
                *result++ = v;
        }
    };
    
    
    template <typename T, typename O>
    eulerian_path<T, O> make_eulerian_path(O output)
    {
        return eulerian_path<T, O>(output);
    }
    
    
    template <typename O>
    struct vertex_depth : public boost::default_dfs_visitor
    {
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
}
