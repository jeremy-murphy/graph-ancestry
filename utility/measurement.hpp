#ifndef MEASUREMENT_HPP
#define MEASUREMENT_HPP

#include <boost/test/unit_test.hpp>
#include <boost/timer/timer.hpp>

#include <vector>
#include <fstream>
#include <cassert>

template <typename N0, typename N1, typename F>
void measure(N0 n, N1 reps, F &f, std::string unit="element")
{
    assert(n > 0);
    boost::timer::cpu_timer timer;
    for(unsigned i = 0; i < reps; i++)
        f();
    timer.stop();
    auto const t = timer.elapsed().user + timer.elapsed().system;
    if (t > 0)
    {
        auto const t_n = double(t) / n / reps;
        auto const test_name = std::string(boost::unit_test::framework::current_test_case().p_name);
        auto const data_file(std::string(".") + test_name);
        BOOST_TEST_MESSAGE(test_name << "[" << reps << " × " << n << "]: " << t_n << " ns per " << unit);
        std::ifstream foo(data_file);
        if(foo.is_open())
        {
            double t0;
            foo >> t0;
            auto const r = t_n / t0 - 1.0;  // regression, as a fraction of 100.
            BOOST_REQUIRE_MESSAGE(r < 0.10, r * 100.0 << "% regression!");
            BOOST_WARN_MESSAGE(r < 0.05, r * 100.0 << "% regression.");
        }
        else
        {
            BOOST_TEST_MESSAGE(data_file << ": not found; starting anew with " << t_n << ".");
            std::ofstream output(data_file);
            output << t_n << "\n";
        }
    }
    else
    {
        BOOST_TEST_MESSAGE("t = " << t << ": aborting.");
    }
}

#endif
