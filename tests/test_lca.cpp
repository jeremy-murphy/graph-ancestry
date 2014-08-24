#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lca.hpp"

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

#include <boost/graph//adjacency_list.hpp>

using namespace LCA;

template <typename Graph>
struct Bender_2005_2
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    
    Graph g;
    
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


BOOST_FIXTURE_TEST_SUITE(TEST_LCA, Bender_2005_2<boost::adjacency_list<>>)

BOOST_AUTO_TEST_CASE(empty)
{
#ifndef NDEBUG
    std::ofstream output("Bender_2005_2.dot");
    boost::write_graphviz(output, g);
#endif
    boost::adjacency_list<> empty;
    preprocess(empty, empty);
}


BOOST_AUTO_TEST_CASE(basic)
{
    boost::adjacency_list<> output;
    preprocess(g, output);
}

BOOST_AUTO_TEST_SUITE_END()
