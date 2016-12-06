#include <vector>

#include "doctest.h"
#include "../ZTransform.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("zTransform()")
{
    vector<float> sine = { 0, 0.70710678118655, 1, 0.70710678118655, 0, -0.70710678118655, -1, -0.70710678118655 };
    auto transfer = zTransform(sine.begin(), sine.end());
    
    CHECK(abs(transfer(0)) == doctest::Approx(0));
    CHECK(abs(transfer(0.78539816339745)) == doctest::Approx(4));
    CHECK(abs(transfer(1.5707963267949)) == doctest::Approx(0));
    CHECK(abs(transfer(2.35619449019234)) == doctest::Approx(0));
    CHECK(abs(transfer(3.141592653589793)) == doctest::Approx(0));
}
