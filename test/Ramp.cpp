#include "doctest.h"

#include "../Ramp.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Ramp")
{
    SUBCASE("Ramp(phase)")
    {
        CHECK(Ramp<float>(0.2).getPhase() == doctest::Approx(0.2));
        CHECK(Ramp<float>(0.8).getPhase() == doctest::Approx(0.8));
        CHECK(Ramp<float>(1.2).getPhase() == doctest::Approx(0.2));
        CHECK(Ramp<float>(-0.2).getPhase() == doctest::Approx(0.8));
    }

    Ramp<float> ramp;

    SUBCASE("setPhase()")
    {
        ramp.setPhase(0.2);
        CHECK(ramp.getPhase() == doctest::Approx(0.2));

        ramp.setPhase(0.8);
        CHECK(ramp.getPhase() == doctest::Approx(0.8));

        ramp.setPhase(1.2);
        CHECK(ramp.getPhase() == doctest::Approx(0.2));

        ramp.setPhase(-0.2);
        CHECK(ramp.getPhase() == doctest::Approx(0.8));
    }

    SUBCASE("increment()")
    {
        for (auto i = 0; i < 10; ++i)
        {
            ramp.increment(0.23);
            
            CHECK(ramp.getPhase() < 1.f);
            CHECK(ramp.getPhase() >= 0.f);
        }
    }
    
    SUBCASE("end")
    {
        auto flag = false;
        ramp.end = [&](){ flag = true; };
        ramp.increment(1.1);
        
        CHECK(flag == true);
    }
}