#include <vector>

#include "doctest.h"

#include "../SpectralCentroid.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Spectral Centroid Bin")
{
    SUBCASE("equal values")
    {
        vector<float> x{1, 1, 1, 1, 1};
        auto centroid = spectralCentroidBin(x.begin(), x.end());
        
        CHECK(centroid == doctest::Approx(2));
    }
    
    SUBCASE("value at middle bin")
    {
        vector<float> x{0, 0, 1, 0, 0};
        auto centroid = spectralCentroidBin(x.begin(), x.end());
        
        CHECK(centroid == doctest::Approx(2));
    }
    
    SUBCASE("value at last bin")
    {
        vector<float> x{0, 0, 0, 0, 1};
        auto centroid = spectralCentroidBin(x.begin(), x.end());
        
        CHECK(centroid == doctest::Approx(4));
    }
    
    
}