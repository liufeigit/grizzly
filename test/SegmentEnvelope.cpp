#include <vector>

#include "doctest.h"

#include <iostream>

#include "../SegmentEnvelope.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Segment Envelope")
{
    SegmentEnvelope<float> env;
    env.addSegment(1, 0.5);
    env.addSegment(0.1, 0.5);
    
    env[0].destination = 2.2;
    
    auto sampleRate = 100;
    
    vector<float> out(sampleRate);
    auto inc = 1.0 / sampleRate;
    
    for (auto i = 0; i < out.size(); ++i)
    {
        out[i] = env.read();
        env.increment(inc);
    }
    
    out.emplace_back(env.read());
    
    SUBCASE("index 50 == value 2.2")
    {
        CHECK(out[50] == doctest::Approx(2.2));
    }
    
    SUBCASE("last index has value 0.1")
    {
        CHECK(out.back() == doctest::Approx(0.1f));
    }
    
    

}
