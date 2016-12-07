#include "doctest.h"

#include "../Dynamic.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Dynamic")
{
    SUBCASE("Make-up gain")
    {
        CHECK(compressorMakeUpGain(-24, 2.f).value == doctest::Approx(6));
        CHECK_THROWS_AS(compressorMakeUpGain(-24, 0.f), std::invalid_argument);
    }
    
    SUBCASE("compress")
    {
        CHECK(compressDownFactor(0, -6, 2, 0).value== doctest::Approx(-3));
        CHECK(compressDownFactor(-6, 0, 2, 0).value == 0);
        CHECK(compressDownFactor(0, -6, 0.5, 0).value == doctest::Approx(6));
        CHECK_THROWS_AS(compressDownFactor(0, -6, 0, 0), std::invalid_argument);
    }
    
    SUBCASE("expand")
    {
        CHECK(expandDownFactor(0, -6, 2, 0).value == 0);
        CHECK(expandDownFactor(-6, 0, 2, 0).value == doctest::Approx(-3));
        CHECK(expandDownFactor(-6, 0, 0.5, 0).value == doctest::Approx(6));
        CHECK_THROWS_AS(expandDownFactor(-6, 0, 0, 0), std::invalid_argument);
    }
}
