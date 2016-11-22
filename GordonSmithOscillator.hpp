//
//  GordonSmithOscillator.hpp
//  bear-dsp
//
//  Created by Stijn Frishert on 7/13/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP
#define GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP

#include <cmath>
#include <unit/hertz.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Sine approximation using Gordon-Smith
    template <class T>
    class GordonSmithOscillator
    {
    public:
        //! Construct the oscillator
        GordonSmithOscillator() = default;
        
        //! Construct the oscillator
        GordonSmithOscillator(unit::hertz<float> frequency) :
            frequency(frequency)
        {
            recomputeEpsilon();
        }
        
        //! Compute the next sample
        T operator()()
        {
            yq -= epsilon * y;
            
            return y += epsilon * yq;
        }
        
        //! Change the frequency
        void setFrequency(unit::hertz<float> frequency)
        {
            this->frequency = frequency;
            recomputeEpsilon();
        }
        
        //! Set the sample rate
        void setSampleRate(unit::hertz<float> sampleRate)
        {
            this->sampleRate = sampleRate;
            recomputeEpsilon();
        }
        
    private:
        //! Recompute epsilon
        void recomputeEpsilon()
        {
            auto theta = frequency / sampleRate * bear::TWO_PI<float>;
            
            epsilon = 2 * sin(theta / 2);
            y = sin(-theta);
            yq = cos(-theta);            
        }
        
    private:
        //! The last calculated y
        T y = 0;
        
        //! The last calculaced quadratic y (90 degrees from y)
        T yq = 0;
        
        //! The Gordon-Smith epsilon = 2 * sin(theta / 2)
        double epsilon = 0;
        
        //! The frequency of the sine
        unit::hertz<float> frequency;
        
        //! The sample rate at which the object runs
        unit::hertz<float> sampleRate = 44100;
    };
}

#endif /* GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP */
