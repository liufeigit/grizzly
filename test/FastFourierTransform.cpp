#include "doctest.h"

#include "../Apple/FastFourierTransformAccelerate.hpp"
#include "../Ooura/FastFourierTransformOoura.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("FastFourierTransform")
{
    FastFourierTransformAccelerate ooura(8);
    
    SUBCASE("Real")
    {
        SUBCASE("Float")
        {
            vector<float> realInputFloat = {0, 0.707106781186548, 1, 0.707106781186548, 0, -0.707106781186548, -1, -0.707106781186548};
            vector<float> realOutputFloat = {0, 0, 0, 0, 0};
            vector<float> imaginaryOutputFloat = {0, -4, 0, 0, 0};
            
            SUBCASE("Forward")
            {
                vector<float> real(5, 0);
                vector<float> imaginary(5, 0);
                
                ooura.forward(realInputFloat.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 5; ++i)
                {
                    CHECK(real[i] == doctest::Approx(realOutputFloat[i]));
                    CHECK(imaginary[i] == doctest::Approx(imaginaryOutputFloat[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<float> output(8, 0);
                
                ooura.inverse(realOutputFloat.data(), imaginaryOutputFloat.data(), output.data());
                
                for (auto i = 0; i < 8; ++i)
                    CHECK(output[i] == doctest::Approx(realInputFloat[i]));
            }
        }
        
        SUBCASE("Double")
        {
            vector<double> realInputDouble = {0, 0.707106781186548, 1, 0.707106781186548, 0, -0.707106781186548, -1, -0.707106781186548};
            vector<double> realOutputDouble = {0, 0, 0, 0, 0};
            vector<double> imaginaryOutputDouble = {0, -4, 0, 0, 0};
            
            SUBCASE("Forward")
            {
                vector<double> real(5, 0);
                vector<double> imaginary(5, 0);
                
                ooura.forward(realInputDouble.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 5; ++i)
                {
                    CHECK(real[i] == doctest::Approx(realOutputDouble[i]));
                    CHECK(imaginary[i] == doctest::Approx(imaginaryOutputDouble[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<double> output(8, 0);
                
                ooura.inverse(realOutputDouble.data(), imaginaryOutputDouble.data(), output.data());
                
                for (auto i = 0; i < 8; ++i)
                    CHECK(output[i] == doctest::Approx(realInputDouble[i]));
            }
        }
    }
}
