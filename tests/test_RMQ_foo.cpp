#include <boost/config.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <vector>

#include "RMQ_foo.hpp"

using namespace general;

BOOST_AUTO_TEST_SUITE(RMQ_foo)

BOOST_AUTO_TEST_CASE(default_constructible)
{
    sparse_table<std::vector<unsigned>> m;
}

BOOST_AUTO_TEST_CASE(construct_from_array)
{
    std::vector<unsigned> const a = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    sparse_table<std::vector<unsigned>> const foo = make_sparse_table(a);
}

BOOST_AUTO_TEST_CASE(equality_comparable)
{
    std::vector<unsigned> const a = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16}, b = {2, 7, 6, 8, 4, 5, 9};
    sparse_table<std::vector<unsigned>> const m(a), n(a), o(b);
    BOOST_CHECK(m == n);
    BOOST_CHECK(m != o);
}


BOOST_AUTO_TEST_CASE(copy_constructible)
{
    std::vector<unsigned> const a = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    sparse_table<std::vector<unsigned>> const m(a), n(m);
    BOOST_CHECK(m == n);
}

BOOST_AUTO_TEST_CASE(copy_assignable)
{
    std::vector<unsigned> const a = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    sparse_table<std::vector<unsigned>> const m(a), n = m;
    BOOST_CHECK(m == n);
}

BOOST_AUTO_TEST_CASE(query)
{
    std::vector<unsigned> const a = {2, 7, 6, 8, 4, 5, 9, 1, 10, 11, 3, 7, 19, 4, 11, 16};
    sparse_table<std::vector<unsigned>> const m(a);
    unsigned x = m.query(0u, 15u);
    BOOST_CHECK_EQUAL(x, 7);
}

BOOST_AUTO_TEST_SUITE_END()
