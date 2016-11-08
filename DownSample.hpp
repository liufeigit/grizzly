//
//  DownSample.hpp
//  libbear
//
//  Created by Milan van der Meer on 23/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_DOWN_SAMPLE_HPP
#define BEAR_DSP_DOWN_SAMPLE_HPP

#include <cstddef>
#include <gsl/span>
#include <stdexcept>
#include <vector>

#include "Parallel.hpp"
#include "Window.hpp"

namespace bear::dsp
{
    //! Downsample a vector of floats to a single float
    template <class T>
    class DownSample
    {
    public:
        //! Construct the downsample
        /*! @param factor The amount of oversampling that takes place (1 is none, 2 is twice as big)
            @param size The size of the filter kernel */
        DownSample(std::size_t factor, std::size_t size);
        
        //! Downsample a vector of floats (size factor) to a single float
        T process(const std::vector<T>& x);
        
        //! Down sample a range of floats
        std::vector<T> processSpan(const std::vector<T>& x);
        
        //! Set the down-sampling factor and recompute the filter
        void setFactor(size_t factor);
        
        //! Retrieve the up-sampling factor
        inline auto getFactor() const { return factor; }
        
        //! The size of the filter kernel
        inline auto getFilterSize() const { return filterSize; }
        
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
    
    template <class T>
    T DownSample<T>::process(const std::vector<T>& x)
    {
        if (x.size() != factor)
            throw std::runtime_error("Input is not of factor size");
        
        // Write the input to the delay
        for (auto& sample : x)
        {
            delayLine.insert(delayLine.begin(), sample);
            delayLine.pop_back();
        }
        
        
        // Initialize the output
        T output = 0;
        
        for (auto sample = 0; sample < factor; ++sample)
        {
            float temp = 0;
            
            dot(const std::vector<float>&(delayLine.data() + sample, numberOfSteps), factor,
                const std::vector<float>&(filterKernel.data() + sample, numberOfSteps), factor,
                temp);
            
            output += temp;
        }
        
        return output;
    }
    
    template <class T>
    std::vector<T> DownSample<T>::processSpan(const std::vector<T>& x)
    {
        std::vector<T> out;
        out.reserve(x.size() / factor);
        
        for (auto i = 0; i < x.size(); i += factor)
            out.emplace_back(process({&x[i], (long)factor}));
        
        return out;
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
        filterKernel = createSymmetricSincWindow(filterSize, 0.5 / factor);
        
        auto window = createSymmetricKaiserWindow(filterSize, betaFactor);
        auto windowedFilter = multiply(gsl::as_cspan(filterKernel), gsl::as_cspan(window));
        
        normalizeSum(windowedFilter);
        filterKernel = windowedFilter;
    }
}

#endif
