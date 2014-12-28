#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include <iterator>
#include <vector>
#include <unordered_map>

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
                        2, 3, 2, 3, 2, 1, 2, 1, 0    };
    std::vector<std::size_t> R_indices = {0,  1, 21, 27,  2, 16, 22, 28, 36,  3, 
                                          5, 11, 17, 23, 29, 31, 33,  6,  8, 12};
    std::unordered_map<std::size_t, std::size_t> R = {{0, 0}, {1, 1}, {2, 21}, {3, 27}, {4, 2}, {5, 16}, {6, 22}, {7, 28}, {8, 36}, {9, 3}, {10, 5}, {11, 11}, {12, 17}, {13, 23}, {14, 29}, {15, 31}, {16, 33}, {17, 6}, {18, 8}, {19, 12}};
    
    // Manually entering iterators is tedious and they are confusing to read.
    std::vector<std::size_t> T_values = {0, 1, 2, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3, 2, 1, 1, 2, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

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


template <typename Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS>>
struct Bender_2005_4
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename boost::graph_traits<Graph>::edge_descriptor edge_descriptor;
    
    Graph g;
    
    Bender_2005_4()
    {
        using boost::add_edge;
        add_edge(0, 5, g);
        add_edge(1, 6, g);
        add_edge(5, 8, g);
        add_edge(6, 8, g);
        add_edge(2, 9, g);
        add_edge(2, 5, g);
        add_edge(3, 7, g);
        add_edge(4, 7, g);
        add_edge(7, 6, g); // weird horizontal arrow
        add_edge(6, 9, g);
        add_edge(7, 10, g);
        add_edge(10, 9, g);
    }
};
