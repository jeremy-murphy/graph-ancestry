#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include "lowest_common_ancestor.hpp"
#include "fixture.hpp"
#include <transformers.hpp>

#ifndef NDEBUG
#include <boost/graph/graphviz.hpp>
#include <fstream>
#endif

#include <boost/graph/adjacency_list.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/multi_array.hpp>

#include <vector>
#include <iostream>
#include <locale>
#include <functional>
#include <random>
#include <unordered_map>

using namespace std;
using namespace graph_algorithms;
using boost::make_indirect_iterator;

struct enable_locale
{
    enable_locale() { cout.imbue(locale("")); cerr.imbue(locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

typedef boost::multi_array_types::extent_range range;

BOOST_FIXTURE_TEST_SUITE(TEST_LCA, Bender_2005_2<boost::adjacency_list<> >)

BOOST_AUTO_TEST_CASE(empty_preprocess)
{
#ifndef NDEBUG
    ofstream output("Bender_2005_2.dot");
    boost::write_graphviz(output, g);
#endif
    boost::adjacency_list<> const input;
    vector<Bender_2005_2<boost::adjacency_list<> >::vertex_descriptor> Euler_tour_index;
    vector<size_t> Euler_tour_level;
    vector<size_t> representative;
    boost::multi_array<size_t, 2> M(boost::extents[0][0]);
    LCA_preprocess(input,
                   boost::make_function_output_iterator(
                     general::make_reflection(Euler_tour_index.begin(),
                                              representative.begin())),
                   Euler_tour_level, M);
    lowest_common_ancestor<boost::adjacency_list<> const> foo(input);

    BOOST_CHECK(Euler_tour_level.empty());
    // BOOST_CHECK(M.empty());
}

BOOST_AUTO_TEST_CASE(basic_preprocess)
{
  int const n = num_vertices(g), m = 2 * n - 1;
  vector<vertex_descriptor> Euler_tour_index(m);
  vector<size_t> Euler_tour_level;
  vector<size_t> representative(n);
  boost::multi_array<size_t, 2> M(general::sparse_table_extent(m));
  LCA_preprocess(g, boost::make_function_output_iterator(
                            general::make_reflection(Euler_tour_index.begin(),
                            representative.begin())), Euler_tour_level, M);
  BOOST_CHECK_EQUAL_COLLECTIONS(Euler_tour_index.begin(), Euler_tour_index.end(),
                                this->E.begin(), this->E.end());
  BOOST_CHECK_EQUAL_COLLECTIONS(Euler_tour_level.begin(), Euler_tour_level.end(),
                                this->L.begin(), this->L.end());
  // BOOST_CHECK_EQUAL_COLLECTIONS(begin(R), end(R), begin(R), end(R));
  // BOOST_CHECK(foo.representative() == this->R);
    // BOOST_CHECK_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(T_values), end(T_values));
}

#if 0

BOOST_AUTO_TEST_CASE(basic_query)
{
    vector<vertex_descriptor> E;
    vector<size_t> L;
    unordered_map<size_t, size_t> R;
    auto const n = num_vertices(g);
    boost::multi_array<size_t, 2> M(general::sparse_table_extent(2 * n - 1));
    LCA_preprocess(g, E, L, inserter(R, end(R)), M);
    // We check just to make sure we did not make a mistake in the test.
    BOOST_REQUIRE_EQUAL_COLLECTIONS(begin(E), end(E), begin(this->E), end(this->E));
    BOOST_REQUIRE_EQUAL_COLLECTIONS(begin(L), end(L), begin(this->L), end(this->L));
    BOOST_REQUIRE(R == this->R);
    // BOOST_REQUIRE_EQUAL_COLLECTIONS(make_indirect_iterator(begin(T)), make_indirect_iterator(end(T)), begin(T_values), end(T_values));
    vector<size_t> R2(R.size());
    for (auto it = begin(R); it != end(R); ++it)
        R2[it->first] = it->second;
    auto result = lowest_common_ancestor(0u, 0u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 0u);
    result = lowest_common_ancestor(0u, 7u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 0u);
    result = lowest_common_ancestor(11u, 12u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 1u);
    result = lowest_common_ancestor(17u, 19u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 4u);
    result = lowest_common_ancestor(14u, 16u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 7u);
    result = lowest_common_ancestor(12u, 17u, E, L, R2, M);
    BOOST_CHECK_EQUAL(result, 1u);
}

#endif

BOOST_AUTO_TEST_SUITE_END()

