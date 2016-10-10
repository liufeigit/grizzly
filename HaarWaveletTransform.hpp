//
//  HaarWaveletTransform.hpp
//  libbear
//
//  Created by Danny van Swieten on 03/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_HAAR_WAVELET_TRANSFORM_HPP
#define BEAR_DSP_HAAR_WAVELET_TRANSFORM_HPP

#include <cmath>
#include <cstddef>
#include <gsl/span>
#include <stdexcept>
#include <vector>

#include <dsperados/math/utility.hpp>

namespace bear::dsp
{
    // The fast Haar wavelet transform is a series of linear homomorphic filters recursively applied to a signal or part of a signal.
    // Google Haar wavelet for it's basefunction. The input should be a power of 2 in length and will create an output of log2(N).
    
    //! The foward Haar wavelet transform
    /*! The fast Haar wavelet transform is a series of linear homomorphic filters recursively applied to a signal or part of a signal.
        Google Haar wavelet for it's basefunction. The input should be a power of 2 in length and will create an output of log2(N). */
    template <class T>
    constexpr void fastHaarWaveletTransform(gsl::span<T> input)
    {
        if (!isPowerOf2(input.size()))
            throw std::invalid_argument("input size should be the power of 2");
        
        std::size_t i = 1;
        std::size_t j = 2;
        
        const auto numFilters = log2(input.size());
        auto m = numFilters;
        
        for (auto l = 0; l < numFilters; ++l)
        {
            m /= 2;
            
            for (auto k = 0; k < m; ++k)
            {
                const auto a = (input[j * k] + input[j * k + i]) * 0.5;
                const auto c = (input[j * k] - input[j * k + i]) * 0.5;
                
                input[j * k] = a;
                input[j * k + i] = c;
            }
            
            i *= 2;
            j *= 2;
        }
    }
    
    //! The inverse Haar wavelet transform
    template <class T>
    constexpr void inverseFastHaarWaveletTransform(gsl::span<T> input)
    {
        const auto numFilters = input.size();

        auto i = numFilters >> 1;
        auto j = 2 * i;
        auto m = 1;

        for (auto l = 0;  l < numFilters;  ++l)
        {
            for (auto k = 0;  k < m;  ++k)
            {
                const auto a0 = input[j * k] + input[j * k + i];
                const auto a1 = input[j * k] - input[j * k + i];
                
                input[j * k] = a0;
                input[j * k + i] = a1;
            }
            
            i /= 2;
            j /= 2;
            m *= 2;
        }
    }
}

#endif
