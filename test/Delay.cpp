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
        SUBCASE("negative argument")
        {
            Delay<int> delay (-1);
            CHECK (delay.getMaximumDelayTime() == 0);
        }
        
        SUBCASE("initialized to zero")
        {
            Delay<int> delay (4);
            for (int i = 0; i < 5; i++)
            {
                CHECK (delay.read(i) == 0);
            }
        }
    }
    
    SUBCASE("write()")
    {
        SUBCASE("one argument")
        {
            vector<int> values {0,1,2};
            Delay<int> delay (2);
            
            for (auto& value:values)
                delay.write(value);
            
            for (int i = 0; i < 3; i++)
            {
                CHECK(delay.read(i) == values[i]);
            }
        }
        
        SUBCASE("multiple arguments")
        {
            Delay<complex<int>> delay(2);
            
            for (int i = 0; i < 3; i++)
            {
                delay.write(i,i);
            }
            for (int i = 0; i < 3; i++)
            {
                CHECK(delay.read(i) == complex<int> (2,2));
                CHECK(delay.read(i).imag() == i);
//                CHECK(delay.read(i).real() == i);
            }
        }
    }
    
//    SUBCASE("read()")
//    {
//        SUBCASE("standard interpolation")
//        {
//            Delay<float> delay(1);
//            for (auto& x: vector<float>{0,1})
//            {
//                delay.write(x);
//            }
//            CHECK(delay.read(0.5) == 0.5);
//        }
//        
//        
//    }
    
    
}
