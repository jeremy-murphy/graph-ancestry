#include "reflect.hpp"

namespace graph_algorithms
{
    /**
     *  Time complexity: worst-case O(|V|⋅|E|)
     * 
     *  Note: G is also modified.
     */
    template <typename Graph, typename MutableGraph>
    typename boost::graph_traits<Graph>::vertices_size_type
    create_common_ancestor_existence_tc(Graph &G, MutableGraph &F)
    {
        typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
        
        auto const offset = reflect_through_sources(G);
        Graph tmp;
        boost::transitive_closure(G, tmp);
        auto const E = edges(tmp);
        std::for_each(E.first, E.second, [&](edge_descriptor e){ add_edge(source(e, tmp), target(e, tmp), F); });
        
        return offset;
    }
    
    
    template <typename Graph, typename Vertex, typename N>
    bool does_common_ancestor_exist(Vertex u, Vertex v, Graph const &F, N offset)
    {
        return edge(u + offset, v, F).second;
    }
}
