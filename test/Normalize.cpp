#include <vector>

#include "doctest.h"

#include "../Normalize.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("test Normalise.hpp")
{
    SUBCASE("normalise()")
    {
        SUBCASE("Negative peak")
        {
            vector<float> x = {0.1, -0.5};
            normalize(x.begin(), x.end(), x.begin());
            
            SUBCASE("in Bounds")
            {
                for (auto& value: x)
                {
                    CHECK(value <= 1);
                    CHECK(value >= -1);
                }
            }
            
            SUBCASE("aproximates -1")
            {
                float peak = 0;
                
                for (auto& val: x)
                    if (std::abs(val) > peak)
                        peak = std::abs(val);
                
                CHECK(std::abs(peak) == doctest::Approx(1.f));
            }
        }
        
        SUBCASE("Positive peak")
        {
            vector<float> x = {-0.1, 0.5};
            normalize(x.begin(), x.end(), x.begin());
            
            SUBCASE("in Bounds")
            {
                for (auto& value: x)
                {
                    CHECK(value <= 1);
                    CHECK(value >= -1);
                }
            }
            
            SUBCASE("aproximates 1")
            {
                float peak = 0;
                
                for (auto& val: x)
                    if (std::abs(val) > peak)
                        peak = std::abs(val);
                
                CHECK(std::abs(peak) == doctest::Approx(1.f));
            }
        }
    }
    
    SUBCASE("normaliseArea()")
    {
        SUBCASE("Area equal to one")
        {
            vector<float> x{0,5, 1, 0,5};
            normalizeArea(x.begin(), x.end(), x.begin());
            
            auto sum = accumulate(x.begin(), x.end(), 0.f);
            
            CHECK(sum == doctest::Approx(1.f));
        }
        
        SUBCASE("Throw when input has integral of zero")
        {
            vector<float> x{1, -1};
            CHECK_THROWS_AS(normalizeArea(x.begin(), x.end(), x.begin()), std::runtime_error);
        }
    }
    
    
}