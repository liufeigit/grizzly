#include "doctest.h"

#include "../Biquad.hpp"

using namespace dsp;
using namespace std;

#include <iostream>

TEST_CASE("Biquad")
{
    SUBCASE("BiquadDirectFormI")
    {
        BiquadDirectFormI<float> filter;

        filter.coefficients.a0 = 0.1;
        filter.coefficients.a1 = 0.2;
        filter.coefficients.a2 = 0.3;
        filter.coefficients.b1 = 0.4;
        filter.coefficients.b2 = 0.5;
        
        filter.increment(1);
        CHECK(filter.read() == doctest::Approx(0.1));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.16));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.186));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(-0.1544));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(-0.03124));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.0897));
    }
    
    SUBCASE("BiquadTransposedDirectFormII")
    {
        BiquadTransposedDirectFormII<float> filter;
        
        filter.coefficients.a0 = 0.1;
        filter.coefficients.a1 = 0.2;
        filter.coefficients.a2 = 0.3;
        filter.coefficients.b1 = 0.4;
        filter.coefficients.b2 = 0.5;
        
        filter.increment(1);
        CHECK(filter.read() == doctest::Approx(0.1));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.16));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.186));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(-0.1544));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(-0.03124));
        
        filter.increment(0);
        CHECK(filter.read() == doctest::Approx(0.0897));
    }

    SUBCASE("Coefficient setup")
    {
        BiquadCoefficients<float> coefficients;
        
        SUBCASE("throughPass()")
        {
            throughPass(coefficients);
            
            CHECK(coefficients.a0 == doctest::Approx(1));
            CHECK(coefficients.a1 == doctest::Approx(0));
            CHECK(coefficients.a2 == doctest::Approx(0));
            CHECK(coefficients.b1 == doctest::Approx(0));
            CHECK(coefficients.b2 == doctest::Approx(0));
        }
        
        SUBCASE("lowPass()")
        {
            lowPass(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.25136438));
            CHECK(coefficients.a1 == doctest::Approx(0.50272876));
            CHECK(coefficients.a2 == doctest::Approx(0.25136438));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
        
        SUBCASE("highPass()")
        {
            highPass(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.336979717));
            CHECK(coefficients.a1 == doctest::Approx(-0.673959434));
            CHECK(coefficients.a2 == doctest::Approx(0.336979717));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
        
        SUBCASE("bandPassConstantSkirt()")
        {
            bandPassConstantSkirt(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.291040719));
            CHECK(coefficients.a1 == doctest::Approx(0));
            CHECK(coefficients.a2 == doctest::Approx(-0.291040719));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
        
        SUBCASE("bandPassConstantPeak()")
        {
            bandPassConstantPeak(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.411655873));
            CHECK(coefficients.a1 == doctest::Approx(0));
            CHECK(coefficients.a2 == doctest::Approx(-0.411655873));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
        
        SUBCASE("peakConstantSkirt()")
        {
            peakConstantSkirt(coefficients, 44100, 10000, 0.707, 6);
            
            CHECK(coefficients.a0 == doctest::Approx(1.32968616));
            CHECK(coefficients.a1 == doctest::Approx(-0.194630221));
            CHECK(coefficients.a2 == doctest::Approx(0.00780280912));
            CHECK(coefficients.b1 == doctest::Approx(-0.194630221));
            CHECK(coefficients.b2 == doctest::Approx(0.337488979));
        }
        
        SUBCASE("peakConstantQ()")
        {
            peakConstantQ(coefficients, 44100, 10000, 0.707, -6);
            
            CHECK(coefficients.a0 == doctest::Approx(0.854829788));
            CHECK(coefficients.a1 == doctest::Approx(-0.146373048));
            CHECK(coefficients.a2 == doctest::Approx(0.151038364));
            CHECK(coefficients.b1 == doctest::Approx(-0.146373048));
            CHECK(coefficients.b2 == doctest::Approx(0.0058681583));
        }
        
        SUBCASE("lowShelf()")
        {
            lowShelf(coefficients, 44100, 10000, 0.707, 6);
            
            CHECK(coefficients.a0 == doctest::Approx(1.37153018));
            CHECK(coefficients.a1 == doctest::Approx(0.0419876873));
            CHECK(coefficients.a2 == doctest::Approx(0.235444129));
            CHECK(coefficients.b1 == doctest::Approx(-0.36927399));
            CHECK(coefficients.b2 == doctest::Approx(0.195712686));
        }
        
        SUBCASE("highShelf()")
        {
            highShelf(coefficients, 44100, 10000, 0.707, 6);
            
            CHECK(coefficients.a0 == doctest::Approx(1.45477104));
            CHECK(coefficients.a1 == doctest::Approx(-0.537209094));
            CHECK(coefficients.a2 == doctest::Approx(0.284717143));
            CHECK(coefficients.b1 == doctest::Approx(0.0306137521));
            CHECK(coefficients.b2 == doctest::Approx(0.171665296));
        }
        
        SUBCASE("notch()")
        {
            notch(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.588344097));
            CHECK(coefficients.a1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.a2 == doctest::Approx(0.588344097));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
        
        SUBCASE("allPass()")
        {
            allPass(coefficients, 44100, 10000, 0.707);
            
            CHECK(coefficients.a0 == doctest::Approx(0.176688224));
            CHECK(coefficients.a1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.a2 == doctest::Approx(1));
            CHECK(coefficients.b1 == doctest::Approx(-0.171230644));
            CHECK(coefficients.b2 == doctest::Approx(0.176688224));
        }
    }
}
