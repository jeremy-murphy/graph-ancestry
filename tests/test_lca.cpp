#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lca.hpp"

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

#include <boost/graph//adjacency_list.hpp>

using namespace std;
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
    // ofstream output("Bender_2005_2.dot");
    // boost::write_graphviz(output, g);
#endif
    boost::adjacency_list<> const empty;
    boost::adjacency_list<> result;
    preprocess(empty, result);
}


BOOST_AUTO_TEST_CASE(basic)
{
    boost::adjacency_list<> output;
    preprocess(g, output);
}


BOOST_AUTO_TEST_CASE(vertex_height)
{
    vector<size_t> L;
    boost::depth_first_search(g, boost::visitor(detail::make_vertex_level(back_inserter(L))));
    vector<size_t> const expected = {0, 1, 2, 3, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 2, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 1, 0};
    BOOST_CHECK_EQUAL(L.size(), expected.size());
    if(L.size() == expected.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(expected), end(expected));
}


BOOST_AUTO_TEST_CASE(Eulerian_path)
{
    typedef typename boost::graph_traits<decltype(g)>::vertex_descriptor vertex_descriptor;
    vector<vertex_descriptor> E;
    boost::depth_first_search(g, boost::visitor(detail::make_eulerian_path<vertex_descriptor>(back_inserter(E))));
    vector<vertex_descriptor> const expected = {0, 1, 4, 9, 4, 10, 17, 10, 18, 10, 4, 11, 19, 11, 4, 1, 5, 12, 5, 1, 0, 2, 6, 13, 6, 2, 0, 3, 7, 14, 7, 15, 7, 16, 7, 3, 8, 3, 0};
    BOOST_CHECK_EQUAL(E.size(), expected.size());
    if(E.size() == expected.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(expected), end(expected));
}


BOOST_AUTO_TEST_CASE(representative_element_all)
{
    vector<unsigned> a, b;
    auto bi = back_inserter(b);
    auto result = representative_element(begin(a), a.size(), bi);
    BOOST_CHECK_EQUAL(b.size(), 0);
    
    for(unsigned i = 10; i < 20; i++)
    {
        a.push_back(i);
        a.push_back(i);
    }
    
    result = representative_element(begin(a), a.size(), bi);
    vector<size_t> expected = {0, 2, 4, 6, 8, 10, 12, 14, 16, 18};
    BOOST_CHECK_EQUAL(expected.size(), b.size());
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(expected), end(expected), begin(b), end(b));
}

BOOST_AUTO_TEST_SUITE_END()
