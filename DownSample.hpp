//
//  DownSample.hpp
//  libbear
//
//  Created by Milan van der Meer on 23/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_DOWN_SAMPLE_HPP
#define GRIZZLY_DOWN_SAMPLE_HPP

#include <cstddef>
#include <dsperados/math/linear.hpp>
#include <dsperados/math/normalize.hpp>
#include <stdexcept>
#include <vector>

#include "CircularBuffer.hpp"
#include "Window.hpp"

namespace dsp
{
    //! Downsample a container to a single scalar
    template <class T>
    class DownSample
    {
    public:
        //! Construct the downsample
        /*! @param factor The amount of oversampling that takes place (1 is none, 2 is twice as big)
            @param size The size of the filter kernel */
        DownSample(std::size_t factor, std::size_t size);
        
        //! Downsample a container (size factor) to a single scalar
        template <typename Iterator>
        T process(Iterator begin);
        
        //! Downsample a container (size factor) to a single scalar
        template <typename Iterator>
        T operator()(Iterator begin) { return process(begin); }
        
        //! Set the down-sampling factor and recompute the filter
        void setFactor(size_t factor);
        
        //! Retrieve the up-sampling factor
        auto getFactor() const { return factor; }
        
        //! The size of the filter kernel
        auto getFilterSize() const { return filterSize; }
        
        //! Set the beta factor for shaping the Kaiser window
        void setBetaFactor(float beta);
        
    private:
        //! Recompute the kernel matrix (polyphase)
        void recomputeFilter();
        
    private:
        //! The down-sampling factor
        std::size_t factor = 4;
        
        //! The filter size
        const std::size_t filterSize = 64;
        
        //! The number of steps to hop through the filter (filterSize / factor)
        std::size_t numberOfSteps = 16;

        //! The filter kernel
        std::vector<T> filterKernel;
        
        //! Delay line for storing the input samples
        CircularBuffer<T> delayLine;
       
        //! The beta factor for shaping the Kaiser window
        float betaFactor = 8.6;
    };
    
    template <class T>
    DownSample<T>::DownSample(std::size_t factor, std::size_t filterSize) :
        factor(factor),
        filterSize(filterSize),
        numberOfSteps(filterSize / factor),
        delayLine(filterSize)
    {
        if (filterSize % factor != 0)
            throw std::invalid_argument("Filter size must be a multiple of the factor");
        
        recomputeFilter();
    }
    
    template <typename T>
    template <typename Iterator>
    T DownSample<T>::process(Iterator begin)
    {
        // Write the input to the delay
        for (auto i = 0; i < numberOfSteps; ++i)
            delayLine.emplace_back(*begin++);
        
        // Initialize the output
        T output = 0;
        
        for (auto sample = 0; sample < factor; ++sample)
            output += math::dot(delayLine.rbegin(), factor, filterKernel.begin() + sample, factor, numberOfSteps);
        
        return output * factor;
    }
    
    template <class T>
    void DownSample<T>::setFactor(std::size_t factor)
    {
        this->factor = factor;
        numberOfSteps = filterSize / factor;
        recomputeFilter();
    }
    
    template <class T>
    void DownSample<T>::setBetaFactor(float beta)
    {
        betaFactor = beta;
        recomputeFilter();
    }
    
    template <class T>
    void DownSample<T>::recomputeFilter()
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
