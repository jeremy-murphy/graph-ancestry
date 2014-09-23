#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "graph_visitors.hpp"
#include "fixture.hpp"

#include <iostream>
#include <locale>

using namespace graph_algorithms;
using namespace std;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

BOOST_FIXTURE_TEST_SUITE(TEST_GRAPH_VISITORS, Bender_2005_2<boost::adjacency_list<>>)

BOOST_AUTO_TEST_CASE(test_vertex_depth)
{
    vector<size_t> L;
    boost::depth_first_search(g, boost::visitor(make_vertex_depth(back_inserter(L))));
    BOOST_CHECK_EQUAL(L.size(), this->L.size());
    if(L.size() == this->L.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
}


BOOST_AUTO_TEST_CASE(test_eulerian_path)
{
    vector<vertex_descriptor> E;
    boost::depth_first_search(g, boost::visitor(make_eulerian_path<vertex_descriptor>(back_inserter(E))));
    BOOST_CHECK_EQUAL(E.size(), this->E.size());
    if(E.size() == this->E.size())
        BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
}

BOOST_AUTO_TEST_SUITE_END()
