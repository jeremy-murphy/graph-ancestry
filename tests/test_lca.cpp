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

typedef std::vector<std::size_t> index_vector;

template <typename Graph>
struct Bender_2005_2
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    
    Graph g;
    std::vector<vertex_descriptor> E = {0, 1, 4, 9, 4, 10, 17, 10, 18, 10, 4, 11, 19, 11, 4, 1, 5, 12, 5, 1, 0, 2, 6, 13, 6, 2, 0, 3, 7, 14, 7, 15, 7, 16, 7, 3, 8, 3, 0};
    index_vector L = {0, 1, 2, 3, 2, 3, 4, 3, 4, 3, 2, 3, 4, 3, 2, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 2, 3, 2, 3, 2, 1, 2, 1, 0};
    index_vector R;
    typedef typename index_vector::const_iterator const_iterator;
    std::vector<const_iterator> T;
    
    
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
    std::vector<vertex_descriptor> E;
    index_vector L;
    index_vector R;
    typedef typename index_vector::const_iterator const_iterator;
    std::vector<const_iterator> T;
    preprocess(empty, E, L, back_inserter(R), T);
}


BOOST_AUTO_TEST_CASE(basic)
{
    std::vector<vertex_descriptor> E;
    index_vector L;
    index_vector R;
    typedef typename index_vector::const_iterator const_iterator;
    std::vector<const_iterator> T;
    preprocess(g, E, L, back_inserter(R), T);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
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

BOOST_AUTO_TEST_SUITE_END()
