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
#include <unit/radian.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Sine approximation using Gordon-Smith
    template <class T>
    class GordonSmithOscillator
    {
    public:
        //! Construct the oscillator
        constexpr GordonSmithOscillator() = default;
        
        //! Construct the oscillator
        constexpr GordonSmithOscillator(unit::radian<float> angle)
        {
            setAngle(angle);
        }
        
        //! Compute the next sample
        constexpr T process()
        {
            yq -= epsilon * y;
            
            return y += epsilon * yq;
        }
        
        //! Compute the next sample
        constexpr T operator()()
        {
            return process();
        }
        
        //! Change the angle the oscillator should increment every process
        constexpr void setAngle(unit::radian<float> angle)
        {
            epsilon = 2 * sin(angle / 2);
            y = sin(-angle);
            yq = cos(-angle);
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
    };
}

#endif /* GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP */
