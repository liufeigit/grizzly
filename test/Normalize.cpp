#include <algorithm>
#include <vector>

#include "doctest.h"

#include "../Normalize.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Normalize")
{
    SUBCASE("normalise()")
    {
        SUBCASE("negative")
        {
            vector<float> x = {0.1, -0.5};
            normalize(x.begin(), x.end(), x.begin());
            
            SUBCASE("values in bounds")
            {
                for (auto& value: x)
                {
                    CHECK(value <= 1);
                    CHECK(value >= -1);
                }
            }
            
            SUBCASE("peak equals -1")
            {
                auto peak = *min_element(x.begin(), x.end());
                CHECK(peak == doctest::Approx(-1.f));
            }
        }
        
        SUBCASE("positive")
        {
            vector<float> x = {-0.1, 0.5};
            normalize(x.begin(), x.end(), x.begin());
            
            SUBCASE("values in bounds")
            {
                for (auto& value: x)
                {
                    CHECK(value <= 1);
                    CHECK(value >= -1);
                }
            }
            
            SUBCASE("peak equals 1")
            {
                auto peak = *max_element(x.begin(), x.end());
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