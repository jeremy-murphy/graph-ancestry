#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lca.hpp"
#include "fixture.hpp"

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
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

BOOST_AUTO_TEST_CASE(empty_preprocess)
{
#ifndef NDEBUG
    ofstream output("Bender_2005_2.dot");
    boost::write_graphviz(output, g);
#endif
    boost::adjacency_list<> const input;
    vector<vertex_descriptor> E;
    vector<size_t> L;
    vector<size_t> R;
    vector<const_iterator> T;
    lca_preprocess(input, E, L, back_inserter(R), T);
    BOOST_CHECK(E.empty());
    BOOST_CHECK(L.empty());
    BOOST_CHECK(R.empty());
    BOOST_CHECK(T.empty());
}


BOOST_AUTO_TEST_CASE(basic_preprocess)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    vector<size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, back_inserter(R), T);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(R), end(R), begin(this->R_indices), end(this->R_indices));
    BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(this->T_values), end(this->T_values));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    vector<size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, back_inserter(R), T);
    auto result = lca_query(0u, 0u, E, L, R, T);
    BOOST_CHECK_EQUAL(result, 0);
    result = lca_query(0u, 7u, E, L, R, T);
    BOOST_CHECK_EQUAL(result, 0);
    result = lca_query(11u, 12u, E, L, R, T);
    BOOST_CHECK_EQUAL(result, 1);
    result = lca_query(17u, 19u, E, L, R, T);
    BOOST_CHECK_EQUAL(result, 4);
    result = lca_query(14u, 16u, E, L, R, T);
    BOOST_CHECK_EQUAL(result, 7);
}

BOOST_AUTO_TEST_SUITE_END()
