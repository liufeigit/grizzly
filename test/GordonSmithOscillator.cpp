#include <cmath>

#include "doctest.h"

#include "../GordonSmithOscillator.hpp"

using namespace dsp;
using namespace std;

#include <iostream>

TEST_CASE("GordonSmithOscillator")
{
    const double increment = 0.0626893772145; // Corresponds to 440Hz at sample rate 44100Hz
    
    GordonSmithOscillator<float> osc(increment);
    for (double angle = 0; angle <= 6.28318530717959; angle += increment)
        CHECK(osc() == doctest::Approx(sin(angle)).epsilon(0.00125));
}
