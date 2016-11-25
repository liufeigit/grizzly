#include <vector>

#include "doctest.h"

#include "../SpectralCentroid.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Spectral Centroid")
{
    SUBCASE("equal values")
    {
        vector<float> x{1, 1, 1, 1, 1};
        auto centroid = spectralCentroid(x, 10000);
        
        CHECK(centroid == doctest::Approx(4000));
    }
    
    SUBCASE("value at second bin")
    {
        vector<float> x{0, 1, 0, 0, 0};
        auto centroid = spectralCentroid(x, 10000);
        
        CHECK(centroid == doctest::Approx(2000));
    }
}