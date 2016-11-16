//
//  UpSample.hpp
//  libbear
//
//  Created by Milan van der Meer on 23/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_UP_SAMPLE_HPP
#define BEAR_DSP_UP_SAMPLE_HPP

#include <cstddef>
#include <dsperados/math/linear.hpp>
#include <stdexcept>
#include <vector>

#include "Window.hpp"

namespace bear::dsp
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
        std::vector<T> process(T x);
        
        //! Upsample a range of floats
        std::vector<T> processSpan(const std::vector<T>& x);
        
        //! Set the up-sampling factor and recompute the filter
        void setFactor(std::size_t factor);
        
        //! Retrieve the up-sampling factor
        inline auto getFactor() const { return factor; }
        
        //! The size of the filter kernel
        inline auto getFilterSize() const { return filterSize; }
        
        //! Set the beta factor for shaping the Kaiser window
        void setBetaFactor(float beta);
        
    private:
        //! Recompute the matrix
        void recomputeFilter();
        
    private:
        //! The up-sampling factor
        std::size_t factor = 4;
        
        //std::vector<float> kernel;
        std::size_t filterSize = 64;
        
        //! The number of steps to hop through the filter (filterSze / factor)
        std::size_t numberOfSteps = 16;
        
        //! The filter kernel
        std::vector<T> filterKernel;
        
        //! Delay line for storing the input samples
        std::vector<T> delayLine;
        
        //! The beta factor for shaping the Kaiser window
        float betaFactor = 5;
        
    };
    
    template <class T>
    UpSample<T>::UpSample(std::size_t factor, std::size_t filterSize) :
        factor(factor),
        filterSize(filterSize),
        numberOfSteps(filterSize / factor),
        delayLine(numberOfSteps)
    {
        if (filterSize % factor != 0)
            throw std::invalid_argument("Filter size must be a multiple of the factor");
        
        recomputeFilter();
    }
    
    template <class T>
    std::vector<T> UpSample<T>::process(T x)
    {
        // Write the input to the delay
        delayLine.insert(delayLine.begin(), x);
        delayLine.pop_back();
        
        // Initialize the output
        std::vector<T> output(factor);
        
        for (auto sample = 0; sample < output.size(); ++sample)
        {
            float temp = 0;
            
            math::dot(const std::vector<T>&(delayLine.data(), numberOfSteps), 1,
                const std::vector<T>&(filterKernel.data() + sample, numberOfSteps), factor,
                temp);
            
            output[sample] = temp;
        }
        
        return output;
    }
    
    template <class T>
    std::vector<T> UpSample<T>::processSpan(const std::vector<T>& x)
    {
        std::vector<T> out;
        out.reserve(factor * x.size());
        
        for (auto& sample : x)
        {
            for (auto& sample : process(sample))
                out.emplace_back(sample);
        }
        
        return out;
    }
    
    template <class T>
    void UpSample<T>::setFactor(std::size_t factor)
    {
        this->factor = factor;
        numberOfSteps = filterSize / factor;
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
        filterKernel = createSymmetricSincWindow(filterSize, 0.5 / factor);
        
        // Construct the window
        auto window = createSymmetricKaiserWindow(filterSize, betaFactor);
        
        // Multiply kernel with window
        std::transform(filterKernel.begin(), filterKernel.end(), window.begin(), filterKernel.begin(), std::multiplies<>()):
        
        // Make integral of kernel equal to one
        normalizeSum(filterKernel);
    }
}

#endif
