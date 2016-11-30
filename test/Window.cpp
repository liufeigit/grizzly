#include <algorithm>
#include <vector>

#include "doctest.h"

#include "../Window.hpp"

using namespace dsp;
using namespace std;

TEST_CASE("Window")
{
    SUBCASE("rectangular")
    {
        SUBCASE("default")
        {
            auto window = createRectangularWindow<float>(4);
            for(auto& value : window)
                CHECK(value == doctest::Approx(1));
        }
        
        SUBCASE("with argument")
        {
            auto window = createRectangularWindow<float>(4, 2.3);
            
            for(auto& value : window)
                CHECK(value == doctest::Approx(2.3));
        }
    }
    
    SUBCASE("triangle")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricTriangleWindow<float>(10);
            
            CHECK(window[4] == window[5]);
        }
        
        SUBCASE("periodic")
        {
            auto window = createTriangleWindow<float>(10);
            float peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(1));
        }
    }
    
    SUBCASE("Hanning")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricHanningWindow<float>(10);
            
            CHECK(window[4] == window[5]);
        }
        
        SUBCASE("periodic")
        {
            auto window = createHanningWindow<float>(10);
            float peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(1));
        }
    }
    
    SUBCASE("Hamming")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricHammingWindow<float>(10);
            
            CHECK(window[4] == window[5]);
        }
        
        SUBCASE("periodic")
        {
            auto window = createHammingWindow<float>(10);
            float peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(1));
        }
    }
    
    SUBCASE("Blackman")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricBlackmanWindow<float>(10);
            
            CHECK(window[4] == window[5]);
        }
        
        SUBCASE("periodic")
        {
            auto window = createBlackmanWindow<float>(10);
            float peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(1));
        }
    }
    
    SUBCASE("sinc")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricSincWindow<float>(10, M_PI);
            
            CHECK(window[4] == window[5]);
            
            window = createSymmetricSincWindow<float>(10, 0.1);
            
            CHECK(window[4] == window[5]);
        }
        
        SUBCASE("periodic")
        {
            auto window = createSincWindow<float>(10, M_PI);
            float peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(1));
            
            window = createSincWindow<float>(10, 0.1);
            peakValue = *max_element(window.begin(), window.end());
            
            CHECK(peakValue == doctest::Approx(0.1 / M_PI));
        }
    }
    
    SUBCASE("Kaiser")
    {
        SUBCASE("symmetric")
        {
            auto window = createSymmetricKaiserWindow<float>(10, 1);
            
            CHECK(window[4] == window[5]);
        }
    }
}
