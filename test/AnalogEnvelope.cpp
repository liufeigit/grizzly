#include <algorithm>
#include <vector>

#include "doctest.h"

#include "../AnalogEnvelope.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Analog Envelope")
{
    auto sampleRate = 10000;
    AnalogEnvelope<float> envelope(0.0, 0.0, 0.0, 0.0, sampleRate);
    
    envelope.setAttackTime(0.1, 10000);
    envelope.setDecayTime(0.1, 10000);
    envelope.setReleaseTime(0.1, 10000);
    envelope.setSustain(0.5);
    
    vector<float> output(10000);
    
    envelope.start();
    
    for (auto i = 0; i < output.size(); ++i)
    {
        if (i == 5000)
            envelope.release();
        
        output[i] = envelope();
    }
    
    SUBCASE("peak at one")
    {
        CHECK(*max_element(output.begin(), output.end()) == doctest::Approx(1.f));
    }
    
    SUBCASE("sustain at 0.5")
    {
        CHECK(output[4000] == doctest::Approx(0.5f));
    }
    
    SUBCASE("releases to zero")
    {
        CHECK(output[7500] == doctest::Approx(0.f));
    }
    
}