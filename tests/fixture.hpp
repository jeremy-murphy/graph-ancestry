#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <iterator>
#include <vector>

typedef std::vector<std::size_t> index_vector;

/// Figure 2 from the paper cited in lca.hpp.
template <typename Graph>
struct Bender_2005_2
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
    typedef typename index_vector::const_iterator const_iterator;

    Graph g;
    std::vector<vertex_descriptor> E = {0,  1 , 4,  9, 4, 10, 17, 10, 18, 10,
                                        4, 11, 19, 11, 4,  1,  5, 12,  5,  1,
                                        0,  2,  6, 13, 6,  2,  0,  3,  7, 14,
                                        7, 15,  7, 16, 7,  3,  8,  3,  0
                                        };
    index_vector L = {  0, 1, 2, 3, 2, 3, 4, 3, 4, 3,
                        2, 3, 4, 3, 2, 1, 2, 3, 2, 1,
                        0, 1, 2, 3, 2, 1, 0, 1, 2, 3,
                        2, 3, 2, 3, 2, 1, 2, 1, 0
                     };
    std::vector<const_iterator> R = {std::begin(E), std::begin(E) + 1, std::begin(E) + 2, std::begin(E) + 3, std::begin(E) + 5, std::begin(E) + 6, std::begin(E) + 8, std::begin(E) + 11, std::begin(E) + 12, std::begin(E) + 16, std::begin(E) + 17, std::begin(E) + 21, std::begin(E) + 22, std::begin(E) + 23, std::begin(E) + 27, std::begin(E) + 28, std::begin(E) + 29, std::begin(E) + 31, std::begin(E) + 33, std::begin(E) + 36};
    std::vector<std::size_t> R_indices = {0, 1, 2, 3, 5, 6, 8, 11, 12, 16, 17, 21, 22, 23, 27, 28, 29, 31, 33, 36};
    // Manually entering iterators is tedious and they are confusing to read.
    std::vector<std::size_t> T_values = {0, 1, 2, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3, 2, 1, 1, 2, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<std::size_t> T_index = {0, 1, 2, 2, 2};
    std::vector<const_iterator> T_iterator;

    Bender_2005_2()
    {
        for(auto i = 0u; i < 19u; ++i)
            boost::add_vertex(g);

        boost::add_edge(0, 1, g);
        boost::add_edge(0, 2, g);
        boost::add_edge(0, 3, g);
        boost::add_edge(1, 4, g);
        boost::add_edge(1, 5, g);
        boost::add_edge(2, 6, g);
        boost::add_edge(3, 7, g);
        boost::add_edge(3, 8, g);
        boost::add_edge(4, 9, g);
        boost::add_edge(4, 10, g);
        boost::add_edge(4, 11, g);
        boost::add_edge(5, 12, g);
        boost::add_edge(6, 13, g);
        boost::add_edge(7, 14, g);
        boost::add_edge(7, 15, g);
        boost::add_edge(7, 16, g);
        boost::add_edge(10, 17, g);
        boost::add_edge(10, 18, g);
        boost::add_edge(11, 19, g);
    }
};


