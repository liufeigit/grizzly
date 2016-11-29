#include <algorithm>
#include <vector>

#include "doctest.h"

#include "../AnalogEnvelope.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Analog Envelope")
{
    auto sampleRate = 10000;
    AnalogEnvelope<float> envelope(sampleRate, 0.0, 0.0, 0.0, 0.0);
    
    envelope.setAttackTime(0.1);
    envelope.setDecayTime(0.1);
    envelope.setReleaseTime(0.1);
    envelope.setSustain(0.5);
    
    vector<float> output(sampleRate);
    
    envelope.start();
    
    for (auto i = 0; i < output.size(); ++i)
    {
        if (i == 0.5 * sampleRate)
            envelope.release();
        
        output[i] = envelope();
    }
    
    SUBCASE("peak at one")
    {
        CHECK(*max_element(output.begin(), output.end()) == doctest::Approx(1.f).epsilon(0.005));
    }
    
    SUBCASE("sustain at 0.5")
    {
        CHECK(output[0.4 * sampleRate] == doctest::Approx(0.5f));
    }
    
    SUBCASE("releases to zero")
    {
        CHECK(output[0.75 * sampleRate] == doctest::Approx(0.f));
    }
    
}
