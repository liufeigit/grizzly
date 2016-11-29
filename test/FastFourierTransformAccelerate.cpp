#include "doctest.h"

#include "../Apple/FastFourierTransformAccelerate.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("FastFourierTransformAccelerate")
{
    FastFourierTransformAccelerate fft(8);
    
    SUBCASE("Float")
    {
        vector<float> inputReal = {0, 0.707106781186548, 1, 0.707106781186548, 0, -0.707106781186548, -1, -0.707106781186548};
        
        SUBCASE("Real")
        {
            vector<float> outputReal = {0, 0, 0, 0, 0};
            vector<float> outputImaginary = {0, -4, 0, 0, 0};
            
            SUBCASE("Forward")
            {
                vector<float> real(5, 0);
                vector<float> imaginary(5, 0);
                
                fft.forward(inputReal.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 5; ++i)
                {
                    CHECK(real[i] == doctest::Approx(outputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(outputImaginary[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<float> output(8, 0);
                
                fft.inverse(outputReal.data(), outputImaginary.data(), output.data());
                
                for (auto i = 0; i < 8; ++i)
                    CHECK(output[i] == doctest::Approx(inputReal[i]));
            }
        }
        
        SUBCASE("Complex")
        {
            vector<float> inputImaginary = {0, 0, 0, 0, 0, 0, 0, 0};
            vector<float> outputReal = {0, 0, 0, 0, 0, 0, 0, 0};
            vector<float> outputImaginary = {0, -4, 0, 0, 0, 0, 0, 4};
            
            SUBCASE("Forward")
            {
                vector<float> real(8);
                vector<float> imaginary(8);
                
                fft.forwardComplex(inputReal.data(), inputImaginary.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 8; ++i)
                {
                    CHECK(real[i] == doctest::Approx(outputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(outputImaginary[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<float> real(8);
                vector<float> imaginary(8);
                
                fft.inverseComplex(outputReal.data(), outputImaginary.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 8; ++i)
                {
                    CHECK(real[i] == doctest::Approx(inputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(inputImaginary[i]));
                }
            }
        }
    }
    
    SUBCASE("Double")
    {
        vector<double> inputReal = {0, 0.707106781186548, 1, 0.707106781186548, 0, -0.707106781186548, -1, -0.707106781186548};
        
        SUBCASE("Real")
        {
            vector<double> outputReal = {0, 0, 0, 0, 0};
            vector<double> outputImaginary = {0, -4, 0, 0, 0};
            
            SUBCASE("Forward")
            {
                vector<double> real(5, 0);
                vector<double> imaginary(5, 0);
                
                fft.forward(inputReal.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 5; ++i)
                {
                    CHECK(real[i] == doctest::Approx(outputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(outputImaginary[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<double> output(8, 0);
                
                fft.inverse(outputReal.data(), outputImaginary.data(), output.data());
                
                for (auto i = 0; i < 8; ++i)
                    CHECK(output[i] == doctest::Approx(inputReal[i]));
            }
        }
        
        SUBCASE("Complex")
        {
            vector<double> inputImaginary = {0, 0, 0, 0, 0, 0, 0, 0};
            vector<double> outputReal = {0, 0, 0, 0, 0, 0, 0, 0};
            vector<double> outputImaginary = {0, -4, 0, 0, 0, 0, 0, 4};
            
            SUBCASE("Forward")
            {
                vector<double> real(8);
                vector<double> imaginary(8);
                
                fft.forwardComplex(inputReal.data(), inputImaginary.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 8; ++i)
                {
                    CHECK(real[i] == doctest::Approx(outputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(outputImaginary[i]));
                }
            }
            
            SUBCASE("Inverse")
            {
                vector<double> real(8);
                vector<double> imaginary(8);
                
                fft.inverseComplex(outputReal.data(), outputImaginary.data(), real.data(), imaginary.data());
                
                for (auto i = 0; i < 8; ++i)
                {
                    CHECK(real[i] == doctest::Approx(inputReal[i]));
                    CHECK(imaginary[i] == doctest::Approx(inputImaginary[i]));
                }
            }
        }
    }
}
