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
    lca_preprocess(input, E, L, R, T);
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
    lca_preprocess(g, E, L, R, T);
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_CHECK_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    // BOOST_CHECK_EQUAL_COLLECTIONS(begin(R), end(R), begin(R), end(R));
    BOOST_CHECK(R == this->R);
    // auto const t = [](pair<size_t, size_t> const &x){ return "{" + to_string(x.first) + ", " + to_string(x.second) + "}"; };
    // copy(boost::make_transform_iterator(begin(R), t), boost::make_transform_iterator(end(R), t), ostream_iterator<string>(cerr, ", ")); cerr << endl;
    BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(T_values), end(T_values));
}


BOOST_AUTO_TEST_CASE(basic_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, R, T);
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


#ifdef NDEBUG

#include "measurement.hpp"

#include <boost/function_output_iterator.hpp>
#include <algorithm>
#include <random>
#include <boost/timer/timer.hpp>
#include <fstream>
#include <string>


BOOST_FIXTURE_TEST_SUITE(measure_LCA, Bender_2005_2<boost::adjacency_list<>>)

BOOST_AUTO_TEST_CASE(measure_lca_preprocess)
{
    auto f = [&]()
    {
        vector<vertex_descriptor> E;
        vector<size_t> L;
        unordered_map<size_t, size_t> R;
        vector<const_iterator> T;
        return lca_preprocess(g, E, L, R, T);
    };
    measure(boost::num_vertices(g), 1u << 20, f);
}


BOOST_AUTO_TEST_CASE(measure_lca_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    vector<const_iterator> T;
    lca_preprocess(g, E, L, R, T);
    vector<size_t> R2(R.size());
    for (auto it = begin(R); it != end(R); it != end(R))
        R2[it->second] = it->first;
    auto f = std::bind(lca_query<unsigned, decltype(E), decltype(L), decltype(R2), decltype(T)>, 12u, 17u, E, L, R2, T);
    measure(1, 1ul << 30, f, "query");
}


BOOST_AUTO_TEST_SUITE_END()

#endif
