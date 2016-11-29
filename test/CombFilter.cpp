#include <algorithm>
#include <vector>

#include "doctest.h"

#include "../CombFilter.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("CombFilter")
{
    SUBCASE("FeedBackCombFilter")
    {
        SUBCASE("constructor")
        {
            SUBCASE("is initialized to 0")
            {
                FeedBackCombFilter<int> filter (4);
                
                REQUIRE (filter.getMaxDelayTime() == 4);
                
                for (int i = 0; i < 4; i++)
                    CHECK(filter(0., 0., 0.) == 0);
            }
        }
        
        SUBCASE("process()")
        {
            SUBCASE("Impulse")
            {
                FeedBackCombFilter<double> filter (2);
                
                CHECK(filter(1., 1., 0.5) == doctest::Approx(1.));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.5));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.25));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.125));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.0625));
            }
            
            SUBCASE("postDelay")
            {
                FeedBackCombFilter<double> filter (2);
                filter.postDelay = [&](const double& x)
                {
                    static double xHistory = 0.;
                    auto sum = x + xHistory;
                    xHistory = x;
                    return sum / 2.;
                };
                
                CHECK(filter(1., 1., 0.5) == doctest::Approx(1.0));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.25));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.3125));
            }
        }
    }
    SUBCASE("FeedForwardCombFilter")
    {
        SUBCASE("constructor")
        {
            SUBCASE("is initialized to 0")
            {
                FeedForwardCombFilter<int> filter (4);
                
                REQUIRE (filter.getMaxDelayTime() == 4);
                
                for (int i = 0; i < 4; i++)
                    CHECK(filter(0., 0., 0.) == 0);
            }
        }
        
        SUBCASE("process()")
        {
            SUBCASE("Impulse")
            {
                FeedForwardCombFilter<double> filter (2);
                
                CHECK(filter(1., 1., 0.5) == doctest::Approx(1.));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.5));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.));
            }
            
            SUBCASE("postDelay")
            {
                FeedForwardCombFilter<double> filter (2);
                filter.postDelay = [&](const double& x)
                {
                    static double xHistory = 0.;
                    auto sum = x + xHistory;
                    xHistory = x;
                    return sum / 2.;
                };
                
                CHECK(filter(1., 1., 0.5) == doctest::Approx(1.0));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.25));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.25));
                CHECK(filter(0., 1., 0.5) == doctest::Approx(0.));
            }
        }
    }
}
