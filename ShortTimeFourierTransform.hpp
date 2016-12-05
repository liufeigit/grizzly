//
//  ShortTimeFourierTransform.hpp
//  libbear
//
//  Created by Stijn on 04/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

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
