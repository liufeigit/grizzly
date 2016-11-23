#include <stdexcept>

#include "doctest.h"

#include "../WaveShaper.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("WaveShaper")
{
    SUBCASE("sigmoid")
    {
        CHECK(sigmoid(-1, 1, 1) == doctest::Approx(-1));
        CHECK(sigmoid(0, 1, 1) == 0);
        CHECK(sigmoid(1, 1, 1) == doctest::Approx(1));
    }
    
    SUBCASE("sigmoidTan")
    {
        CHECK(sigmoidTan(-1, 1, 1) == doctest::Approx(-1));
        CHECK(sigmoidTan(0, 1, 1) == 0);
        CHECK(sigmoidTan(1, 1, 1) == doctest::Approx(1));
    }
    
    SUBCASE("sigmoidExp")
    {
        CHECK(sigmoidExp(-1, 1, 1) == doctest::Approx(-1));
        CHECK(sigmoidExp(0, 1, 1) == 0);
        CHECK(sigmoidExp(1, 1, 1) == doctest::Approx(1));
    }
    
    SUBCASE("throw for distortion factor <= 0")
    {
        CHECK_THROWS_AS(sigmoid(0, 0, 0), std::runtime_error);
        CHECK_THROWS_AS(sigmoidTan(0, 0, 0), std::runtime_error);
        CHECK_THROWS_AS(sigmoidExp(0, 0, 0), std::runtime_error);
    }
}