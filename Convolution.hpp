//
//  Convolution.hpp

//  libbear
//
//  Created by Milan van der Meer on 02/03/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_CONVOLUTION_HPP
#define BEAR_DSP_CONVOLUTION_HPP

#include <algorithm>
#include <initializer_list>
#include <gsl/gsl.h>
#include <vector>

#include "Delay.hpp"
#include "Filter.hpp"

namespace bear::dsp
{
    //! Convolution, in the mathematical sense
    template <class T>
    class Convolution : public Filter<T>
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
        Convolution(gsl::span<const T> kernel)
        {
            setKernel(kernel);
        }
        
        //! Process a single sample
        T process(const T& x) override final
        {
            // Write the input into the delay line
            delay.write(x);
            
            // Convolve the past N samples with the kernel and sum them
            T sum = 0;
            for (auto h = 0; h < kernel.size(); ++h)
                sum += kernel[h] * delay.read(h);
            
            return sum;
        }
        
        //! Process multiple samples
        std::vector<T> processor(gsl::span<const T> x)
        {
            std::vector<T> y(x.size());
            std::transform(x.begin(), x.end(), y.begin(), &Convolution::process);
        }
        
        //! Change the kernel
        void setKernel(gsl::span<const T> kernel)
        {
            this->kernel.assign(kernel.begin(), kernel.end());
            delay.resize(kernel.size());
        }
        
    private:
        //! Delay line used for input
        Delay<T> delay;
        
        //! The convolution kernel
        std::vector<T> kernel;
    };
}


#endif
