/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

#ifndef GRIZZLY_SHORT_TIME_FOURIER_TRANSFORM_HPP
#define GRIZZLY_SHORT_TIME_FOURIER_TRANSFORM_HPP

#include <cstddef>
#include <cmath>
#include <complex>
#include <experimental/optional>
#include <stdexcept>
#include <vector>

#include "Spectrum.hpp"

namespace dsp
{
    class FastFourierTransformBase;
    
    //! The short-time Fourier transform
    template <typename InputIterator, typename WindowIterator>
    std::vector<Spectrum<float>> shortTimeFourierTransform(InputIterator begin, InputIterator end, FastFourierTransformBase& fourier, std::experimental::optional<WindowIterator> windowBegin, size_t hopSize)
    {
        const auto frameSize = fourier.getSize();
        
        // Generate space for all spectra
        const auto size = std::distance(begin, end);
        std::vector<Spectrum<float>> spectra;
        spectra.reserve(std::ceil(size / hopSize));
        
        // Go through all frames
        for (auto i = 0; i < size; i += hopSize)
        {
            // Cut out the frame
            const auto it = begin + i;
            std::vector<float> frame(it, (i + frameSize < size) ? it + frameSize : end);
            frame.resize(frameSize);
            
            // Multiply the frame with the window if we have one
            if (windowBegin)
                std::transform(frame.begin(), frame.end(), *windowBegin, frame.begin(), [](const float& lhs, const float& rhs){ return lhs * rhs; });
            
            spectra.emplace_back(fourier.forward(frame.data()));
        }
        
        return spectra;
    }
    
    //! The short-time Fourier transform
    template <typename InputIterator, typename WindowIterator>
    std::vector<Spectrum<float>> shortTimeFourierTransform(InputIterator begin, InputIterator end, std::size_t frameSize, std::experimental::optional<WindowIterator> windowBegin, size_t hopSize)
    {
        FastFourierTransform fft(frameSize);
        return shortTimeFourierTransform(begin, end, fft, windowBegin, hopSize);
    }
}

#endif
