#include <vector>

#include "doctest.h"
#include "../HighFrequencyContent.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("HighFrequencyContent")
{
    vector<float> in = { 1, 3, 0.5, 2, 0.5 };
    
    CHECK(highFrequencyContentBrossier(in.begin(), in.end()) == doctest::Approx(2.4));
    CHECK(highFrequencyContentMasri(in.begin(), in.end()) == doctest::Approx(4.5));
    CHECK(highFrequencyContentJensen(in.begin(), in.end()) == doctest::Approx(6.2));
}
