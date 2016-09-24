//
//  Buffer.hpp
//  libbear
//
//  Created by Stijn Frishert on 9/3/15.
//  Copyright (c) 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_BUFFER_HPP
#define BEAR_DSP_BUFFER_HPP

#include <algorithm>
#include <cstddef>
#include <gsl/span.h>
#include <vector>

#include <unit/hertz.hpp>

namespace bear::dsp
{
    //! A buffer of samples, sampled at a given rate
    template <class T>
    class Buffer
    {
    public:
        //! Construct an empty buffer
        Buffer() = default;
        
        //! Construct a buffer with N samples and a sample rate
        Buffer(unit::hertz<float> sampleRate, std::size_t size = 0) : samples(size), sampleRate(sampleRate) { }
        
        //! Construct by passing samples and their sample rate
        Buffer(unit::hertz<float> sampleRate, gsl::span<const float> samples) : samples(samples.begin(), samples.end()), sampleRate(sampleRate) { }
        
        //! Convert from a (differently typed) Buffer
        template <class U>
        Buffer(const Buffer<U>& rhs)
        {
            operator=(rhs);
        }
        
        //! Copy assign from a (differently typed) Buffer
        template <class U>
        Buffer& operator=(const Buffer<U>& rhs)
        {
            samples.resize(rhs.samples.size());
            std::copy(rhs.begin(), rhs.end(), begin());
            
            sampleRate = rhs.sampleRate;
            
            return *this;
        }
        
        // For ranged for-loops
        inline auto begin() { return samples.begin(); }
        inline auto begin() const { return samples.begin(); }
        inline auto end() { return samples.end(); }
        inline auto end() const { return samples.end(); }
        
    public:
        //! The sample data
        std::vector<T> samples;
        
        //! The sample rate
        unit::hertz<float> sampleRate = 44100;
    };
}

#endif // BEAR_DSP_BUFFER_HPP