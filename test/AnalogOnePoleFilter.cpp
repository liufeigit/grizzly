#include "doctest.h"

#include "../AnalogOnePoleFilter.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Analog one-pole filter")
{
    AnalogOnePoleFilter<float> filter;
    filter.setCutOff(1000, 10000);
    
    SUBCASE("process")
    {
        filter.increment(1);
        CHECK(filter.readLowPass() == doctest::Approx(0.24524f));
        CHECK(filter.readHighPass() == doctest::Approx(0.75476f));
        
        filter.increment(0);
        CHECK(filter.readLowPass() == doctest::Approx(0.37019f));
        CHECK(filter.readHighPass() == doctest::Approx(-0.37019f));
        
        filter.increment(0);
        CHECK(filter.readLowPass() == doctest::Approx(0.18862f));
        CHECK(filter.readHighPass() == doctest::Approx(-0.18862f));
        
        filter.increment(0);
        CHECK(filter.readLowPass() == doctest::Approx(0.09611f));
        CHECK(filter.readHighPass() == doctest::Approx(-0.09611f));
        
        filter.increment(0);
        CHECK(filter.readLowPass() == doctest::Approx(0.04897f));
        CHECK(filter.readHighPass() == doctest::Approx(-0.04897f));
        
        filter.increment(0);
        CHECK(filter.readLowPass() == doctest::Approx(0.02495f));
        CHECK(filter.readHighPass() == doctest::Approx(-0.02495f));
    }
    
    SUBCASE("reset")
    {
        filter.reset();
        
        CHECK(filter.getIntegratorState() == doctest::Approx(0));
        CHECK(filter.readLowPass() == doctest::Approx(0));
        CHECK(filter.readHighPass() == doctest::Approx(0));
    }
    
    SUBCASE("set state")
    {
        filter.setState(2);
        
        CHECK(filter.getIntegratorState() == doctest::Approx(2));
        CHECK(filter.readLowPass() == doctest::Approx(2));
        CHECK(filter.readHighPass() == doctest::Approx(0));
    }
}