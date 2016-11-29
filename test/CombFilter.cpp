#include <algorithm>
#include <vector>
#include <iostream>

#include "doctest.h"

#include "../CombFilter.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("CombFilter")
{
    SUBCASE("FeedBackCombFilter")
    {
        SUBCASE("FeedBackCombFilter ()")
        {
            SUBCASE("size is zero")
            {
                FeedBackCombFilter<int> filter = FeedBackCombFilter<int> ();
                CHECK (filter.getMaxDelayTime () == 0);
            }
            SUBCASE("is initialized to 0")
            {
                FeedBackCombFilter<int> filter (4);
                CHECK (filter.getMaxDelayTime () == 4);
                for (int i = 0; i < 4; i++)
                {
                    CHECK(filter(0.,0., 0.) == 0);
                }
            }
        }
        SUBCASE("process ()")
        {
            SUBCASE("overload")
            {
                FeedBackCombFilter<double> filterA (2);
                FeedBackCombFilter<double> filterB (2);
                
                CHECK(filterA(1.,1.,0.5) == filterB.process(1.,1.,0.5));
                CHECK(filterA(1.,1.,0.5) == filterB.process(1.,1.,0.5));
                CHECK(filterA(1.,1.,0.5) == filterB.process(1.,1.,0.5));

            }
            SUBCASE("feedBack")
            {
                FeedBackCombFilter<double> filter (2);
                
                CHECK(filter(1.,1.,0.5) == 1.);
                CHECK (filter(0.,1.,0.5) == 0.5);
                CHECK (filter(0.,1.,0.5) == 0.25);
                CHECK (filter(0.,1.,0.5) == 0.125);
                CHECK (filter(0.,1.,0.5) == 0.0625);
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
                CHECK(filter(1.,1.,0.5) == 1.);
                CHECK(filter(0.,1.,0.5) == 0.25);
                CHECK(filter(0.,1.,0.5) == 0.3125);
                
            }
            
        }
    }
    
}
