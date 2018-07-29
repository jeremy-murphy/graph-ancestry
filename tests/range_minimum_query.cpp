#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <range_minimum_query.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/multi_array.hpp>

#include <boost/array.hpp>

#include <boost/assign.hpp>

#include <boost/iterator.hpp>
#include <boost/container/vector.hpp>

#include <iterator>
#include <locale>
#include <iostream>

using namespace boost;
using namespace general;

struct enable_locale
{
    enable_locale() { std::cout.imbue(std::locale("")); }
};

BOOST_GLOBAL_FIXTURE(enable_locale);

// Simple function to calculate RMQ in linear time.
template <typename C, typename N>
N RMQ_linear(N i, N j, C const &A)
{
    BOOST_CONCEPT_ASSERT((boost::UnsignedInteger<N>));
    // BOOST_CONCEPT_ASSERT((boost::RandomAccessContainer<C>));
    
    return std::min_element(begin(A) + i, begin(A) + j + 1) - begin(A);
}

struct index_8
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<std::size_t, 2> M;
    boost::array<unsigned, 8> A;
    boost::array<std::size_t, 13> Z;
    
    index_8() : M(boost::extents[range(1, 4)][8]),
      A(boost::assign::list_of(2)(7)(6)(8)(4)(5)(9)(1)),
      Z(boost::assign::list_of(0)(2)(2)(4)(4)(5)(7)(0)(4)(4)(4)(7)(7)) {}
};

struct index_7
{
    boost::array<unsigned, 7> A;
    boost::array<std::size_t, 10> Z;
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<std::size_t, 2> M;

    index_7() :
      A(boost::assign::list_of(2)(7)(6)(8)(4)(5)(9)),
      Z(boost::assign::list_of(0)(2)(2)(4)(4)(5)(0)(4)(4)(4)),
      M(boost::extents[range(1, 3)][7]) {}
};

struct index_16
{
    boost::array<unsigned, 16> A;
    boost::array<std::size_t, 38> Z;
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<std::size_t, 2> M;

    index_16() :
      A(boost::assign::list_of(2)(7)(6)(8)(4)(5)(9)(1)(10)(11)(3)(7)(19)(4)(11)(16)),
      Z(boost::assign::list_of(0)(2)(2)(4)(4)(5)(7)(7)(8)(10)(10)(11)(13)(13)(14)// 2
        (0)(4)(4)(4)(7)(7)(7)(7)(10)(10)(10)(13)(13)// 4
        (7)(7)(7)(7)(7)(7)(7)(7)(10)// 8
        (7) // 16
      ),
      M(boost::extents[range(1, 5)][16]) {}
};


BOOST_AUTO_TEST_SUITE(RMQ_index)

BOOST_AUTO_TEST_CASE(test_empty_preprocess)
{
    boost::container::vector<unsigned> Q;
    boost::multi_array<std::size_t, 2> B(boost::extents[0][0]);
    boost::multi_array<std::size_t, 2> const C(B);
    RMQ_preprocess(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(0);
    RMQ_preprocess(Q, B);
    BOOST_CHECK(B == C);
    Q.push_back(1);
    RMQ_preprocess(Q, B);
    BOOST_CHECK(B == C);
}


// typedef boost::mpl::vector<index_8, index_7, index_16> test_fixtures;

BOOST_FIXTURE_TEST_CASE(test_query8, index_8)
{
    range_minimum_query< boost::array<unsigned, 8> > foo(A);
    for(unsigned i = 0; i < A.size(); i++)
        for(unsigned j = i; j < A.size(); j++)
        {
            std::size_t minimum = foo(i, j);
            BOOST_CHECK_EQUAL(minimum, RMQ_linear(i, j, A));
        }
}

BOOST_FIXTURE_TEST_CASE(test_query7, index_7)
{
  range_minimum_query< boost::array<unsigned, 7> > foo(A);
  for(unsigned i = 0; i < A.size(); i++)
        for(unsigned j = i; j < A.size(); j++)
        {
            std::size_t minimum = foo(i, j);
            BOOST_CHECK_EQUAL(minimum, RMQ_linear(i, j, A));
        }
}

BOOST_FIXTURE_TEST_CASE(test_query16, index_16)
{
  range_minimum_query< boost::array<unsigned, 16> > foo(A);
  for(unsigned i = 0; i < A.size(); i++)
        for(unsigned j = i; j < A.size(); j++)
        {
            std::size_t minimum = foo(i, j);
            BOOST_CHECK_EQUAL(minimum, RMQ_linear(i, j, A));
        }
}

BOOST_AUTO_TEST_SUITE_END()


#ifdef NDEBUG

#include "measurement.hpp"

#include <algorithm>
#include <random>

struct random_RMQ
{
    mt19937 engine;
    uniform_int_distribution<unsigned> d;
    vector<unsigned> a;
    typedef BOOST_DEDUCED_TYPENAME vector<unsigned>::const_iterator const_iterator;
    
    random_RMQ() : d(0, (1ul << 32) - 1)
    {
        fill_n(back_inserter(a), 1ul << 22, d(engine));
    }
};

BOOST_FIXTURE_TEST_SUITE(measure_RMQ, random_RMQ)

BOOST_AUTO_TEST_CASE(measure_RMQ_preprocess)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<std::size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    auto const f = [&]()
    {
        RMQ_preprocess(a, M);
    };
    measure(a.size(), 1u << 4, f);
}


BOOST_AUTO_TEST_CASE(measure_RMQ_query)
{
    typedef boost::multi_array_types::extent_range range;
    boost::multi_array<std::size_t, 2> M((boost::extents[range(1, 23)][1 << 22ul]));
    RMQ_preprocess(a, M);
    auto const f = bind(RMQ<std::size_t, decltype(a), decltype(M)>, 0, a.size() - 1, a, M);
    measure(1, 1ul << 29, f, "query");
}

BOOST_AUTO_TEST_SUITE_END()

#endif
