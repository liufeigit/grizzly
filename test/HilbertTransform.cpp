#include <vector>

#include "doctest.h"
#include "../HilbertTransform.hpp"

using namespace dsp;

TEST_CASE("HilbertTransform")
{
	SUBCASE("Forward")
	{
        std::vector<float> in = { 0, 0.70710678118655, 1, 0.70710678118655, 0, -0.70710678118655, -1, -0.70710678118655 };
        std::vector<float> out(8);
        hilbertTransform(in.begin(), in.end(), out.begin(), HilbertTransformDirection::FORWARD);
        
        CHECK(out[0] == doctest::Approx(-1));
        CHECK(out[1] == doctest::Approx(-0.70710678118655));
        CHECK(out[2] == doctest::Approx(0));
        CHECK(out[3] == doctest::Approx(0.70710678118655));
        CHECK(out[4] == doctest::Approx(1));
        CHECK(out[5] == doctest::Approx(0.70710678118655));
        CHECK(out[6] == doctest::Approx(0));
        CHECK(out[7] == doctest::Approx(-0.70710678118655));
	}
    
    SUBCASE("Inverse")
    {
        std::vector<float> in = { -1, -0.70710678118655, 0, 0.70710678118655, 1, 0.70710678118655, 0, -0.70710678118655 };
        std::vector<float> out(8);
        hilbertTransform(in.begin(), in.end(), out.begin(), HilbertTransformDirection::INVERSE);
        
        CHECK(out[0] == doctest::Approx(0));
        CHECK(out[1] == doctest::Approx(0.70710678118655));
        CHECK(out[2] == doctest::Approx(1));
        CHECK(out[3] == doctest::Approx(0.70710678118655));
        CHECK(out[4] == doctest::Approx(0));
        CHECK(out[5] == doctest::Approx(-0.70710678118655));
        CHECK(out[6] == doctest::Approx(-1));
        CHECK(out[7] == doctest::Approx(-0.70710678118655));
    }
}
