#include <vector>

#include "doctest.h"

#include "../Delay.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Delay")
{
    SUBCASE("Delay()")
    {
        Delay<int> delay(4);
        
        REQUIRE(delay.getMaximumDelayTime() == 4);
        
        for (int i = 0; i < 5; i++)
            CHECK(delay.read(i) == 0);
    }
    
    SUBCASE("write()")
    {
        Delay<int> delay(2);
        
        for (auto& value : { 0, 1, 2 })
            delay.write(value);
        
        for (int i = 0; i < 3; ++i)
            CHECK(delay.read(2 - i) == i);
    }
    
    SUBCASE("read()")
    {
        Delay<float> delay(1);
        
        for (auto& x: {0, 1})
            delay.write(x);
        
        CHECK(delay.read(0) == 1);
        CHECK(delay.read(1) == 0);
        CHECK(delay.read(0.2) == doctest::Approx(0.8));
        CHECK(delay.read(0.8) == doctest::Approx(0.2));
        
        REQUIRE_NOTHROW(delay.read(1.2));
        CHECK(delay.read(1.2) == doctest::Approx(0));
        
        REQUIRE_NOTHROW(delay.read(-0.2));
        CHECK(delay.read(-0.2) == doctest::Approx(1));
    }
    
    SUBCASE("resize()")
    {
        Delay<int> delay(1);
        
        delay.write(1);
        delay.write(2);
        
        delay.resize(2);
        REQUIRE(delay.getMaximumDelayTime() == 2);
        
        CHECK(delay.read(0) == 2);
        CHECK(delay.read(1) == 1);
        CHECK(delay.read(2) == 0);
    }
}
