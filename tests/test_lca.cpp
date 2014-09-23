#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lca.hpp"
#include "fixture.hpp"

#ifndef NDEBUG
// #include <boost/graph/graphviz.hpp>
// #include <fstream>
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <vector>
#include <iostream>
#include <locale>

using namespace std;
using namespace graph_algorithms;
using boost::make_indirect_iterator;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

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


BOOST_AUTO_TEST_SUITE_END()
