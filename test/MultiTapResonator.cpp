#include <vector>

#include "doctest.h"
#include "../MultiTapResonator.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("MultiTapResonator")
{
    SUBCASE("MultiTapResonator()")
    {
        MultiTapResonator<float> resonator(20, 2);
        
        REQUIRE(!resonator.empty());
        REQUIRE(resonator.size() == 2);
        REQUIRE(resonator.getMaximumDelayTime() == 20);
    }
    
    SUBCASE("MultiTapResonator() with initializer_list")
    {
        MultiTapResonator<float> resonator = {{10, 0.5}, {20, 0.7}};
        
        REQUIRE(!resonator.empty());
        REQUIRE(resonator.size() == 2);
        REQUIRE(resonator.getMaximumDelayTime() == 20);
        
        REQUIRE(resonator[0].delayTime == 10);
        REQUIRE(resonator[0].feedback == 0.5);
        REQUIRE(resonator[1].delayTime == 20);
        REQUIRE(resonator[1].feedback == 0.7);
    }
}
