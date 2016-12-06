#include <iostream>
#include <vector>

#include "doctest.h"
#include "../DownSample.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("DownSample")
{
    DownSample<float> down(4, 64);
    
    REQUIRE(down.getFilterSize() == 64);
    REQUIRE(down.getFactor() == 4);
    
    SUBCASE("process()")
    {
        const vector<float> in = { 1, 1, 1, 1 };
        
        CHECK(down(in.begin()) == doctest::Approx(0).epsilon(0.1));
        down(in.begin());
        down(in.begin());
        CHECK(down(in.begin()) == doctest::Approx(1).epsilon(0.0002));
    }
    
    SUBCASE("setFactor()")
    {
        down.setFactor(8);
        REQUIRE(down.getFactor() == 8);
        REQUIRE(down.getFilterSize() == 64);
    }
    
    SUBCASE("setBetaFactor()")
    {
        REQUIRE_NOTHROW(down.setBetaFactor(8));
    }
}
