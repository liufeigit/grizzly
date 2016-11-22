#include "doctest.h"

#include "../Ramp.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("test Ramp.hpp")
{
    Ramp<float> ramp;

    SUBCASE("In Bounds")
    {
        for (auto i = 0; i < 10; ++i)
        {
            ramp.increment(0.23);
            
            CHECK(ramp.getPhase() < 1.f);
            CHECK(ramp.getPhase() >= 0.f);
        }
    }
    
    SUBCASE("End function called")
    {
        auto flag = false;
        ramp.end = [&](){ flag = true; };
        ramp.increment(1.1);
        
        CHECK(flag == true);
    }
}