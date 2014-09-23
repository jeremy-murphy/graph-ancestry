#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lca.hpp"

#ifndef NDEBUG
// #include <boost/graph/graphviz.hpp>
// #include <fstream>
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/indirect_iterator.hpp>

using namespace std;
using namespace LCA;
using boost::make_indirect_iterator;

typedef vector<std::size_t> index_vector;

/// Figure 2 from the paper cited in lca.hpp.
template <typename Graph>
struct Bender_2005_2
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor vertex_descriptor;
    typedef typename index_vector::const_iterator const_iterator;
    
    Graph g;
    vector<vertex_descriptor> E = {0,  1 , 4,  9, 4, 10, 17, 10, 18, 10, 
                                        4, 11, 19, 11, 4,  1,  5, 12,  5,  1, 
                                        0,  2,  6, 13, 6,  2,  0,  3,  7, 14, 
                                        7, 15,  7, 16, 7,  3,  8,  3,  0};
    index_vector L = {  0, 1, 2, 3, 2, 3, 4, 3, 4, 3, 
                        2, 3, 4, 3, 2, 1, 2, 3, 2, 1, 
                        0, 1, 2, 3, 2, 1, 0, 1, 2, 3, 
                        2, 3, 2, 3, 2, 1, 2, 1, 0};
    vector<const_iterator> R = {begin(E), begin(E) + 1, begin(E) + 2, begin(E) + 3, begin(E) + 5, begin(E) + 6, begin(E) + 8, begin(E) + 11, begin(E) + 12, begin(E) + 16, begin(E) + 17, begin(E) + 21, begin(E) + 22, begin(E) + 23, begin(E) + 27, begin(E) + 28, begin(E) + 29, begin(E) + 31, begin(E) + 33, begin(E) + 36};
    // Manually entering iterators is tedious and they are confusing to read.
    vector<unsigned> T = {0, 1, 2, 2, 2, 3, 3, 3, 3, 2, 2, 3, 3, 2, 1, 1, 2, 2, 1, 0, 0, 1, 2, 2, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 3, 3, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 2, 2, 2, 2, 1, 1, 1, 0, 0, 1, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    
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
    boost::adjacency_list<> const input;
    vector<vertex_descriptor> E;
    vector<size_t> L;
    vector<const_iterator> R;
    vector<const_iterator> T;
    preprocess(input, E, L, back_inserter(R), T);
    BOOST_CHECK(E.empty());
    BOOST_CHECK(L.empty());
    BOOST_CHECK(R.empty());
    BOOST_CHECK(T.empty());
}


BOOST_AUTO_TEST_CASE(basic)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    vector<const_iterator> R;
    vector<const_iterator> T;
    preprocess(g, E, L, back_inserter(R), T);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(R)), make_indirect_iterator(end(R)), make_indirect_iterator(begin(this->R)), make_indirect_iterator(end(this->R)));
    BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(this->T), end(this->T));
}


BOOST_AUTO_TEST_CASE(vertex_height)
{
    vector<size_t> L;
    boost::depth_first_search(g, boost::visitor(detail::make_vertex_depth(back_inserter(L))));
    BOOST_CHECK_EQUAL(L.size(), this->L.size());
    if(L.size() == this->L.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
}


BOOST_AUTO_TEST_CASE(Eulerian_path)
{
    vector<vertex_descriptor> E;
    boost::depth_first_search(g, boost::visitor(detail::make_eulerian_path<vertex_descriptor>(back_inserter(E))));
    BOOST_CHECK_EQUAL(E.size(), this->E.size());
    if(E.size() == this->E.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
}

BOOST_AUTO_TEST_SUITE_END()
