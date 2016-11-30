#include <iostream>

#include "doctest.h"

#include "../FirstOrderFilter.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("FirstOrderFilter")
{
    FirstOrderFilter<float> filter;
    
    SUBCASE("Manual coefficients")
    {
        filter.coefficients.a0 = 0.5;
        filter.coefficients.a1 = 0.5;
        filter.coefficients.b1 = 0.5;
        
        CHECK(filter.process(1) == doctest::Approx(0.5));
        CHECK(filter.process(0) == doctest::Approx(0.75));
        CHECK(filter.process(0) == doctest::Approx(0.375));
        CHECK(filter.process(0) == doctest::Approx(0.1875));
        CHECK(filter.process(0) == doctest::Approx(0.09375));
        CHECK(filter.process(0) == doctest::Approx(0.046875));
    }
    
    SUBCASE("throughPass()")
    {
        throughPass(filter.coefficients);
        
        CHECK(filter.coefficients.a0 == doctest::Approx(1));
        CHECK(filter.coefficients.a1 == doctest::Approx(0));
        CHECK(filter.coefficients.b1 == doctest::Approx(0));
    }
    
    SUBCASE("lowPassOnePole")
    {
        SUBCASE("with cutoff")
        {
            lowPassOnePole(filter.coefficients, 44100, 10000);
            
            CHECK(filter.coefficients.a0 == doctest::Approx(0.759433464558704));
            CHECK(filter.coefficients.a1 == doctest::Approx(0));
            CHECK(filter.coefficients.b1 == doctest::Approx(0.240566535441296));
        }
        
        SUBCASE("with time and constant")
        {
            lowPassOnePole(filter.coefficients, 44100, 1, 5);
            
            CHECK(filter.coefficients.a0 == doctest::Approx(0.0001133723));
            CHECK(filter.coefficients.a1 == doctest::Approx(0));
            CHECK(filter.coefficients.b1 == doctest::Approx(0.9998866277));
        }
    }
    
    SUBCASE("lowPassOnePoleZero")
    {
        lowPassOnePoleZero(filter.coefficients, 44100, 10000);
        
        CHECK(filter.coefficients.a0 == doctest::Approx(0.379716732279352));
        CHECK(filter.coefficients.a1 == doctest::Approx(0.379716732279352));
        CHECK(filter.coefficients.b1 == doctest::Approx(0.240566535441296));
    }
    
    SUBCASE("highPassOnePoleZero")
    {
        highPassOnePoleZero(filter.coefficients, 44100, 10000);
        
        CHECK(filter.coefficients.a0 == doctest::Approx(0.620283267720648));
        CHECK(filter.coefficients.a1 == doctest::Approx(-0.620283267720648));
        CHECK(filter.coefficients.b1 == doctest::Approx(0.240566535441296));
    }
}
