//
//  Window.hpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_WINDOW_HPP
#define GRIZZLY_WINDOW_HPP

#include <cmath>
#include <dsperados/math/constants.hpp>
#include <vector>
#include <unit/radian.hpp>

#include "Waveform.hpp"

namespace dsp
{
    //! Create a rectangular window
    template <typename T1, typename T2 = T1>
    std::vector<T1> createRectangularWindow(size_t size, T2 amplitude = 1)
    {
        return std::vector<T1>(size, amplitude);
    }
    
    //! Create a triangle window
    template <typename T>
    std::vector<T> createTriangleWindow(size_t size)
    {
        std::vector<T> window(size);
        auto increment = 1.l / size;
        
        for (auto i = 0; i < size; ++i)
            window[i] = generateUnipolarTriangle<T>(increment * i);
        
        return window;
    }
    
    //! Create a symmetric triangle window
    template <typename T>
    std::vector<T> createSymmetricTriangleWindow(size_t size)
    {
        std::vector<T> window(size);
        auto increment = 1.l / (size-1);
        
        for (auto i = 0; i < size; ++i)
            window[i] = generateUnipolarTriangle<T>(increment * i);
        
        return window;
    }
    
    //! Create a Hanning window
    template <typename T>
    std::vector<T> createHanningWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = (1 - cos(math::TWO_PI<long double> * i / size)) * 0.5;
        
        return window;
    }
    
    //! Create a symmetric Hanning window
    template <typename T>
    std::vector<T> createSymmetricHanningWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = (1 - cos(math::TWO_PI<long double> * i / (size - 1))) * 0.5;
        
        return window;
    }
    
    //! Create a Hamming window
    template <typename T>
    std::vector<T> createHammingWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.54 - 0.46 * cos(math::TWO_PI<long double> * i / size);
        
        return window;
    }
    
    //! Create a symmetric Hamming window
    template <typename T>
    std::vector<T> createSymmetricHammingWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.54 - 0.46 * cos(math::TWO_PI<long double> * i / (size - 1));
        
        return window;
    }
    
    //! Create a Blackman window
    template <typename T>
    std::vector<T> createBlackmanWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.42 - 0.5 * cos(math::TWO_PI<long double> * i / size) + 0.08 * cos(2 * math::TWO_PI<long double> * i / size);
        
        return window;
    }
    
    //! Create a symmetric Blackman window
    template <typename T>
    std::vector<T> createSymmetricBlackmanWindow(size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.42 - 0.5 * cos(math::TWO_PI<long double> * i / (size - 1)) + 0.08 * cos(2 * math::TWO_PI<long double> * i / (size - 1));
        
        return window;
    }
    
    // Create a sinc window
    template <typename T>
    std::vector<T> createSincWindow(size_t size, unit::radian<double> cutOff)
    {
        std::vector<T> sinc(size);

        auto halfSize = size / 2.l;
        for (auto i = 0; i < size; ++i)
        {
            auto indexMinusHalfSize = i - halfSize;
            if (indexMinusHalfSize == 0)
                sinc[i] = cutOff / math::PI<double>;
            else
                sinc[i] = sin(cutOff * indexMinusHalfSize) / (math::PI<double> * indexMinusHalfSize);
        }

        return sinc;
    }
    
    //! Create a symmetrical sinc window
    template <typename T>
    std::vector<T> createSymmetricSincWindow(size_t size, unit::radian<double> cutOff)
    {
        std::vector<T> sinc(size);
        
        auto halfSize = (size - 1) / 2.l;
        for (auto i = 0; i < size; ++i)
        {
            auto indexMinusHalfSize = i - halfSize;
            if (indexMinusHalfSize == 0)
                sinc[i] = cutOff / math::PI<double>;
            else
                sinc[i] = sin(cutOff * indexMinusHalfSize) / (math::PI<double> * indexMinusHalfSize);
        }
        
        return sinc;
    }
    
    //! Helper function for Kaiser window
    double besselizero(double x)
    {
        double temp;
        double sum = 1.0;
        double u = 1.0;
        double halfx = x / 2.0;
        size_t n = 1;
        
        do
        {
            temp = halfx / static_cast<double>(n);
            u *= temp * temp;
            sum += u;
            n++;
        }
        while (u >= 1e-21 * sum);
        
        return(sum);
    }
    
    //! Create a symmetric Kaiser window. The Beta factor (>= 1) determines the steepness
    template <typename T>
    std::vector<T> createSymmetricKaiserWindow(size_t size, double betaFactor)
    {
        std::vector<T> window(size);
        double tmp;
        double k1 = 1.0 / besselizero(betaFactor);
        size_t k2 = 1 - (size & 1);
        size_t halfSize = (size + 1) >> 1;
        
        for (auto i = 0; i < halfSize; ++i)
        {
            tmp = static_cast<double>(2 * i + k2) / (static_cast<double>(size) - 1.0);
            window[halfSize - (1 & (!k2)) + i] = window[halfSize - 1 - i] = k1 * besselizero(betaFactor * sqrt(1.0 - tmp * tmp));
        }
        
        return window;
    }
}

#endif /* GRIZZLY_WINDOW_HPP */
