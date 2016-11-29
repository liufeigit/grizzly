//
//  Convolution.hpp

//  libbear
//
//  Created by Milan van der Meer on 02/03/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_CONVOLUTION_HPP
#define GRIZZLY_CONVOLUTION_HPP

#include <algorithm>
#include <initializer_list>
#include <gsl/gsl>
#include <vector>

#include "Delay.hpp"

namespace dsp
{
    //! Convolution, in the mathematical sense
    template <class T>
    class Convolution
    {
    public:
        //! Construct with a kernel
        Convolution(std::initializer_list<T> kernel) :
            Convolution(kernel.begin(), kernel.end())
        {
            
        }
        
        //! Construct with a kernel
        template <typename Iterator>
        Convolution(Iterator begin, Iterator end) :
            delay(std::distance(begin, end)),
            kernel(begin, end)
        {
            
        }
        
        //! Process a single sample
        T process(const T& x)
        {
            // Write the input into the delay line
            delay.write(x);
            
            // Convolve the past N samples with the kernel and sum them
            T sum = 0;
            for (auto h = 0; h < kernel.size(); ++h)
                sum += kernel[h] * delay.read(h);
            
            return sum;
        }
        
        //! Process a single sample
        T operator()(const T& x)
        {
            return process(x);
        }
        
        //! Change the kernel
        template <typename Iterator>
        void setKernel(Iterator begin, Iterator end)
        {
            kernel.assign(begin, end);
            delay.resize(kernel.size());
        }
        
        //! Return the kernel
        const std::vector<T>& getKernel() const { return kernel; }
        
    private:
        //! Delay line used for input
        Delay<T> delay;
        
        //! The convolution kernel
        std::vector<T> kernel;
    };
    
    //! Convolve two buffers, return a buffer with size input + kernel - 1
    template <typename InputIterator, typename KernelIterator>
    static std::vector<std::common_type_t<typename InputIterator::value_type, typename KernelIterator::value_type>>
    convolve(InputIterator inBegin, InputIterator inEnd, KernelIterator kernelBegin, KernelIterator kernelEnd)
    {
        const auto inputSize = std::distance(inBegin, inEnd);
        const auto kernelSize = std::distance(kernelBegin, kernelEnd);
        
        std::vector<std::common_type_t<typename InputIterator::value_type, typename KernelIterator::value_type>> output(inputSize + kernelSize - 1);
        
        InputIterator input = inBegin;
        for (int sample = 0; sample < output.size(); ++sample)
        {
            InputIterator input2 = input++;
            KernelIterator kernel = kernelBegin;
            
            for (int h = 0; h < kernelSize; ++h)
            {
                auto input3 = input2--;
                if (sample - h < 0)
                    continue;
                
                if (sample - h >= inputSize)
                    continue;
                
                output[sample] += (*kernel++) * (*input3);
            }
        }
        
        return output;
    }
}


#endif
