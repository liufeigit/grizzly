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
        //! Construct without a kernel
        Convolution() = default;
        
        //! Construct with a kernel
        Convolution(std::initializer_list<T> kernel)
        {
            setKernel({kernel.begin(), kernel.end()});
        }
        
        //! Construct with a kernel
        Convolution(const std::vector<T>& kernel)
        {
            setKernel(kernel);
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
        
        T operator()(const T& x)
        {
            return process(x);
        }
        
        //! Process multiple samples
        std::vector<T> processor(const std::vector<T>& x)
        {
            std::vector<T> y(x.size());
            std::transform(x.begin(), x.end(), y.begin(), &Convolution::process);
        }
        
        //! Change the kernel
        void setKernel(const std::vector<T>& kernel)
        {
            this->kernel.assign(kernel.begin(), kernel.end());
            delay.resize(kernel.size());
        }
        
        //! Convolve two buffers, return a buffer with size input + kernel - 1
        static std::vector<float> convolve(std::vector<T>& input, std::vector<T>& kernel)
        {
            std::vector<T> output(input.size() + kernel.size() - 1);
            
            for (auto sample = 0; sample < output.size(); ++sample)
            {
                for (auto h = 0; h < kernel.size(); ++h)
                {
                    if (sample - h < 0)
                        continue;
                    if (sample - h >= input.size())
                        continue;
                    
                    output[sample] += kernel[h] * input[sample - h];
                }
            }
            
            return output;
        }
        
    private:
        //! Delay line used for input
        Delay<T> delay;
        
        //! The convolution kernel
        std::vector<T> kernel;
    };
}


#endif
