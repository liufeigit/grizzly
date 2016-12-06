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
#include <limits>
#include <unit/radian.hpp>
#include <vector>

#include "Waveform.hpp"

namespace dsp
{
    //! Create a rectangular window
    template <typename T>
    std::vector<T> createRectangularWindow(std::size_t size, T amplitude = T{1})
    {
        return std::vector<T>(size, amplitude);
    }
    
    //! Create a triangle window
    template <typename T>
    std::vector<T> createTriangleWindow(std::size_t size)
    {
        std::vector<T> window(size);
        auto increment = 1.l / size;
        
        for (auto i = 0; i < size; ++i)
            window[i] = generateUnipolarTriangle<T>(increment * i);
        
        return window;
    }
    
    //! Create a symmetric triangle window
    template <typename T>
    std::vector<T> createSymmetricTriangleWindow(std::size_t size)
    {
        std::vector<T> window(size);
        auto increment = 1.l / (size-1);
        
        for (auto i = 0; i < size; ++i)
            window[i] = generateUnipolarTriangle<T>(increment * i);
        
        return window;
    }
    
    //! Create a Hanning window
    template <typename T>
    std::vector<T> createHanningWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = (1 - cos(math::TWO_PI<long double> * i / size)) * 0.5;
        
        return window;
    }
    
    //! Create a symmetric Hanning window
    template <typename T>
    std::vector<T> createSymmetricHanningWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = (1 - cos(math::TWO_PI<long double> * i / (size - 1))) * 0.5;
        
        return window;
    }
    
    //! Create a Hamming window
    template <typename T>
    std::vector<T> createHammingWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.54 - 0.46 * cos(math::TWO_PI<long double> * i / size);
        
        return window;
    }
    
    //! Create a symmetric Hamming window
    template <typename T>
    std::vector<T> createSymmetricHammingWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.54 - 0.46 * cos(math::TWO_PI<long double> * i / (size - 1));
        
        return window;
    }
    
    //! Create a Blackman window
    template <typename T>
    std::vector<T> createBlackmanWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.42 - 0.5 * cos(math::TWO_PI<long double> * i / size) + 0.08 * cos(2 * math::TWO_PI<long double> * i / size);
        
        return window;
    }
    
    //! Create a symmetric Blackman window
    template <typename T>
    std::vector<T> createSymmetricBlackmanWindow(std::size_t size)
    {
        std::vector<T> window(size);
        
        for (auto i = 0; i < size; ++i)
            window[i] = 0.42 - 0.5 * cos(math::TWO_PI<long double> * i / (size - 1)) + 0.08 * cos(2 * math::TWO_PI<long double> * i / (size - 1));
        
        return window;
    }
    
    // Create a sinc window
    template <typename T>
    std::vector<T> createSincWindow(std::size_t size, unit::radian<double> cutOff)
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
    std::vector<T> createSymmetricSincWindow(std::size_t size, unit::radian<double> cutOff)
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
    
    //! Zeroth-order modified Bessel function (used for the Kaiser window)
    static inline double besseli0(double x)
    {
        const auto x2 = x * x;
        
        // These variables will be updated each iteration
        double y = 1;
        double s = 1;
        int n = 1;
        
        // Officially this goes on until infinity, but we'll accept epsilon as
        while (s > y * std::numeric_limits<double>::epsilon())
        {
            s *= x2 / 4.0 / (n * n);
            y += s;
            n++;
        }
        
        return y;
    }
    
    //! Create an asymmetric Kaiser window.
    /*! @param beta: Larger beta gives lower side-lobe levels, but a wider main lobe and vice versa
        beta of 1 ~= rectangular
        beta of 5 ~= hamming
        beta of 6 ~= hann
        beta of 8.6 ~= blackman */
    template <typename T>
    std::vector<T> createKaiserWindow(std::size_t size, double beta)
    {
        std::vector<T> kaiser(size);
        
        const double bb = besseli0(beta);
        
        for (auto n = 0; n < size; ++n)
            kaiser[n] = besseli0(beta * std::sqrt(4 * n * (size - n)) / size) / bb;
        
        return kaiser;
    }
    
    //! Create a symmetric Kaiser window.
    /*! @param beta: Larger beta gives lower side-lobe levels, but a wider main lobe and vice versa
        beta of 1 ~= rectangular
        beta of 5 ~= hamming
        beta of 6 ~= hann
        beta of 8.6 ~= blackman */
    template <typename T>
    std::vector<T> createSymmetricKaiserWindow(std::size_t size, double beta)
    {
        std::vector<T> kaiser(size);
        
        const auto sizeMinusOne = size - 1;
        const double bb = besseli0(beta);
        
        for (auto n = 0; n < size; ++n)
            kaiser[n] = besseli0(beta * std::sqrt(4 * n * (sizeMinusOne - n)) / (sizeMinusOne)) / bb;
        
        return kaiser;
    }
}

#endif /* GRIZZLY_WINDOW_HPP */
