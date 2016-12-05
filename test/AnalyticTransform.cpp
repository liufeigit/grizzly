#include <vector>

#include "doctest.h"
#include "../AnalyticTransform.hpp"

using namespace dsp;

TEST_CASE("AnalyticTransform")
{
    std::vector<float> in = { 0, 0.70710678118655, 1, 0.70710678118655, 0, -0.70710678118655, -1, -0.70710678118655 };
    std::vector<std::complex<float>> out(8);
    analyticTransform(in.begin(), in.end(), out.begin());
    
    REQUIRE(std::abs(out[0]) == doctest::Approx(1));
    CHECK(out[0].real() == doctest::Approx(0));
    CHECK(out[0].imag() == doctest::Approx(-1));
    
    REQUIRE(std::abs(out[1]) == doctest::Approx(1));
    CHECK(out[1].real() == doctest::Approx(0.70710678118655));
    CHECK(out[1].imag() == doctest::Approx(-0.70710678118655));
    
    REQUIRE(std::abs(out[2]) == doctest::Approx(1));
    CHECK(out[2].real() == doctest::Approx(1));
    CHECK(out[2].imag() == doctest::Approx(0));
    
    REQUIRE(std::abs(out[3]) == doctest::Approx(1));
    CHECK(out[3].real() == doctest::Approx(0.70710678118655));
    CHECK(out[3].imag() == doctest::Approx(0.70710678118655));
    
    REQUIRE(std::abs(out[4]) == doctest::Approx(1));
    CHECK(out[4].real() == doctest::Approx(0));
    CHECK(out[4].imag() == doctest::Approx(1));
    
    REQUIRE(std::abs(out[5]) == doctest::Approx(1));
    CHECK(out[5].real() == doctest::Approx(-0.70710678118655));
    CHECK(out[5].imag() == doctest::Approx(0.70710678118655));
    
    REQUIRE(std::abs(out[6]) == doctest::Approx(1));
    CHECK(out[6].real() == doctest::Approx(-1));
    CHECK(out[6].imag() == doctest::Approx(0));
    
    REQUIRE(std::abs(out[7]) == doctest::Approx(1));
    CHECK(out[7].real() == doctest::Approx(-0.70710678118655));
    CHECK(out[7].imag() == doctest::Approx(-0.70710678118655));
}
