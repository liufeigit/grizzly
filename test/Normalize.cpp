#include <vector>

#include "doctest.h"

#include "../Normalize.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("test Normalise.hpp")
{
    SUBCASE("normalise")
    {
        vector<float> x{0.1, 0.5};
        
        dsp::normalize(x.begin(), x.end(), x.begin());
        
        for (auto& value: x)
        {
            CHECK(value <= 1);
            CHECK(value >= -1);
        }
    }
}