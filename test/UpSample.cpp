#include <vector>

#include "doctest.h"
#include "../UpSample.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("UpSample")
{
    UpSample<float> up(4, 64);
    
    REQUIRE(up.getFilterSize() == 64);
    REQUIRE(up.getFactor() == 4);
    
    SUBCASE("process()")
    {
        auto y = up(1);
        REQUIRE(y.size() == 4);
        CHECK(y.front() == doctest::Approx(0).epsilon(0.00005));
        
        for (auto i = 0; i < 15; ++i)
            y = up(1);
        CHECK(y.back() == doctest::Approx(1));
    }
    
    SUBCASE("setFactor()")
    {
        up.setFactor(8);
        REQUIRE(up.getFactor() == 8);
        REQUIRE(up.getFilterSize() == 64);
    }
    
    SUBCASE("setBetaFactor()")
    {
        REQUIRE_NOTHROW(up.setBetaFactor(8));
    }
}
