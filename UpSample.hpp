//
//  UpSample.hpp
//  libbear
//
//  Created by Milan van der Meer on 23/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_UP_SAMPLE_HPP
#define GRIZZLY_UP_SAMPLE_HPP

#include <cstddef>
#include <dsperados/math/linear.hpp>
#include <dsperados/math/normalize.hpp>
#include <stdexcept>
#include <vector>

#include "CircularBuffer.hpp"
#include "Window.hpp"

namespace dsp
{
    //! Upsamples a single value by a factor
    template <class T>
    class UpSample
    {
    public:
        //! Construct the upsampling object
        /*! @param factor The amount of oversampling that takes place (1 is none, 2 is twice as big)
            @param size The size of the filter kernel */
        UpSample(std::size_t factor, std::size_t size);
        
        //! Upsample a single float to a vector float of size factor
        std::vector<T> process(const T& x);
        
        //! Upsample a single float to a vector float of size factor
        std::vector<T> operator()(const T& x) { return process(x); }
        
        //! Set the up-sampling factor and recompute the filter
        void setFactor(std::size_t factor);
        
        //! Retrieve the up-sampling factor
        auto getFactor() const { return factor; }
        
        //! The size of the filter kernel
        auto getFilterSize() const { return filterSize; }
        
        //! Set the beta factor for shaping the Kaiser window
        /*! See createKaiserWindow() for more information */
        void setBetaFactor(float beta);
        
    private:
        //! Recompute the matrix
        void recomputeFilter();
        
    private:
        //! The up-sampling factor
        std::size_t factor = 4;
        
        //std::vector<float> kernel;
        std::size_t filterSize = 64;
        
        //! The filter kernel
        std::vector<T> filterKernel;
        
        //! Delay line for storing the input samples
        CircularBuffer<T> delayLine;
        
        //! The beta factor for shaping the Kaiser window
        float betaFactor = 8.6;
    };
    
    template <class T>
    UpSample<T>::UpSample(std::size_t factor, std::size_t filterSize) :
        factor(factor),
        filterSize(filterSize),
        delayLine(filterSize / factor)
    {
        if (filterSize % factor != 0)
            throw std::invalid_argument("Filter size must be a multiple of the factor");
        
        recomputeFilter();
    }
    
    template <class T>
    std::vector<T> UpSample<T>::process(const T& x)
    {
        // Write the input to the delay
        delayLine.emplace_back(x);
        
        // Initialize the output
        std::vector<T> output(factor);
        
        for (auto sample = 0; sample < output.size(); ++sample)
            output[sample] = math::dot(delayLine.rbegin(), 1, filterKernel.begin() + sample, factor, delayLine.size()) * factor;
        
        return output;
    }
    
    template <class T>
    void UpSample<T>::setFactor(std::size_t factor)
    {
        this->factor = factor;
        recomputeFilter();
    }
    
    template <class T>
    void UpSample<T>::setBetaFactor(float beta)
    {
        betaFactor = beta;
        recomputeFilter();
    }
    
    template <class T>
    void UpSample<T>::recomputeFilter()
    {
        // Construct the kernel
        filterKernel = createSymmetricSincWindow<T>(filterSize, 0.5 / factor);
        
        // Construct the window
        auto window = createSymmetricKaiserWindow<T>(filterSize, betaFactor);
        
        // Multiply kernel with window
        std::transform(filterKernel.begin(), filterKernel.end(), window.begin(), filterKernel.begin(), std::multiplies<>());
        
        // Make integral of kernel equal to one
        math::normalizeArea(filterKernel.begin(), filterKernel.end(), filterKernel.begin());
    }
}

#endif
