#include <algorithm>
#include <complex>
#include <vector>

#include "doctest.h"

#include "../Delay.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Delay")
{
    SUBCASE("Delay()")
    {

        Delay<int> delay (4);
        for (int i = 0; i < 5; i++)
        {
            CHECK (delay.read(i) == 0);
        }
    }
    
    SUBCASE("write()")
    {
        vector<int> values {0,1,2};
        Delay<int> delay (2);
        
        for (auto& value:values)
            delay.write(value);
        
        for (int i = 0; i < 3; i++)
        {
            CHECK(delay.read(2-i) == values[i]);
        }

    }
    
    SUBCASE("read()")
    {
        SUBCASE("standard interpolation")
        {
            Delay<float> delay(1);
            for (auto& x: vector<float>{0,1})
            {
                delay.write(x);
            }
            CHECK(delay.read(0.5) == 0.5);
        }
        
        SUBCASE("other interpolation")
        {
            Delay<float> delay(1);
            for (auto& x: vector<float>{0,1})
            {
                delay.write(x);
            }
            CHECK(delay.read(0.5,math::NearestInterpolation ()) == 0.);
        }
        
    }
    
    
}
