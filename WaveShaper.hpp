//
//  WaveShaper.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 14/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_DSP_WAVESHAPER_HPP
#define BEAR_DSP_WAVESHAPER_HPP

#include <stdexcept>

#include "Math.hpp"

namespace bear::dsp
{
    template <typename T1, typename T2, typename T3>
    static inline T1 sigmoid(const T1& x, const T2& negativeFactor, const T3& positiveFactor)
    {
        auto yAtX1Negative = (negativeFactor) / (1.0 + std::fabs(negativeFactor));
        auto yAtX1Positive = (positiveFactor) / (1.0 + std::fabs(positiveFactor));
        
        if (x > 0)
            return ((x * positiveFactor) / (1.0 + fabs(x * positiveFactor))) / yAtX1Positive;
        else if (x < 0)
            return ((x * negativeFactor) / (1.0 + fabs(x * negativeFactor))) / yAtX1Negative;
        else
            return 0;
    }
    
    template <typename T1, typename T2, typename T3>
    static inline T1 sigmoidTan(const T1& x, const T2& negativeFactor, const T3& positiveFactor)
    {
        if (x > 0)
            return atan(x * positiveFactor) * (1.0 / atan(positiveFactor));
        else if (x < 0)
            return atan(x * negativeFactor) * (1.0 / atan(negativeFactor));
        else
            return 0;
    }
    
    template <typename T1, typename T2, typename T3>
    T1 sigmoidExp(const T1& x, const T2& negativeFactor, const T3& positiveFactor)
    {
        if (x > 0)
            return (1 - exp(-x * positiveFactor)) / (1 - exp(-positiveFactor)) ;
        else if (x < 0)
            return (-1 + exp(x * negativeFactor)) / (1 - exp(-negativeFactor));
        else
            return 0;
    }
    
    template <typename T1, typename T2>
    static inline T1 hardClip(const T1& x, const T2& ceiling)
    {
        if (x > ceiling)
            return ceiling;
        else if (x < -ceiling)
            return -ceiling;
        else
            return x;
    }
}

#endif /* BEAR_DSP_WAVESHAPER_HPP */
