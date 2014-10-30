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
#include <boost/iterator/transform_iterator.hpp>

#include <boost/lexical_cast.hpp>

#include <vector>
#include <iostream>
#include <locale>
#include <functional>
#include <unordered_map>

using namespace std;
using namespace graph_algorithms;
using boost::make_indirect_iterator;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
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
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(input, E, L, inserter(R, end(R)), T);
    BOOST_CHECK(E.empty());
    BOOST_CHECK(L.empty());
    BOOST_CHECK(R.empty());
    BOOST_CHECK(T.empty());
}


BOOST_AUTO_TEST_CASE(basic_preprocess)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, inserter(R, end(R)), T);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    // BOOST_CHECK_EQUAL_COLLECTIONS(begin(R), end(R), begin(R), end(R));
    BOOST_CHECK(R == this->R);
    BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(T_values), end(T_values));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, inserter(R, end(R)), T);
    // We check just to make sure we did not make a mistake in the test.
    BOOST_REQUIRE_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_REQUIRE_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    BOOST_REQUIRE(R == this->R);
    BOOST_REQUIRE_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(T_values), end(T_values));
    vector<size_t> R2(R.size());
    for (auto it = begin(R); it != end(R); ++it)
        R2[it->first] = it->second;
    auto result = lca_query(0u, 0u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 0);
    result = lca_query(0u, 7u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 0);
    result = lca_query(11u, 12u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 1);
    result = lca_query(17u, 19u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 4);
    result = lca_query(14u, 16u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 7);
    result = lca_query(12u, 17u, E, L, R2, T);
    BOOST_CHECK_EQUAL(result, 1);
}

BOOST_AUTO_TEST_SUITE_END()

template <size_t N, size_t K = 2>
struct random_k_tree
{
    boost::adjacency_list<> g;
    mt19937 engine;
    uniform_int_distribution<size_t> d;
    
    random_k_tree() : d(0, N - 1)
    {
        boost::add_edge(0, 1, g);
        do
        {
            auto const n = boost::num_vertices(g);
            auto const src = d(engine) % n;
            boost::add_edge(src, n, g);
        }
        while (boost::num_vertices(g) != N);
    }
};

#include "measurement.hpp"

BOOST_FIXTURE_TEST_CASE(measure_lca_preprocess_random, random_k_tree<1ul << 20>)
{
#ifndef NDEBUG
    {
        ofstream output("random_k_tree<" + boost::lexical_cast<string>(boost::num_vertices(g)) + ">.dot");
        boost::write_graphviz(output, g);
    }
#endif
    typedef typename vector<size_t>::const_iterator const_iterator;
    vector<size_t> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    
    auto f = [&]()
    {
        lca_preprocess(g, E, L, inserter(R, end(R)), T);
        E.clear();
        L.clear();
        R.clear();
        T.clear();
    };
    measure(boost::num_vertices(g), 1u << 5, f);
}

#ifdef NDEBUG


BOOST_FIXTURE_TEST_SUITE(measure_LCA, Bender_2005_2<boost::adjacency_list<>>)

BOOST_AUTO_TEST_CASE(measure_lca_preprocess)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;

    auto f = [&]()
    {
        lca_preprocess(g, E, L, inserter(R, end(R)), T);
        E.clear();
        L.clear();
        R.clear();
        T.clear();
    };
    measure(boost::num_vertices(g), 1u << 20, f);
}


BOOST_AUTO_TEST_CASE(measure_lca_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, inserter(R, end(R)), T);
    vector<size_t> R2(R.size());
    for (auto it = begin(R); it != end(R); ++it)
        R2[it->first] = it->second;
    auto f = bind(lca_query<unsigned, decltype(E), decltype(L), decltype(R2), decltype(T)>, 12u, 17u, E, L, R2, T);
    measure(1, 1ul << 30, f, "query");
}

BOOST_AUTO_TEST_SUITE_END()

#endif
