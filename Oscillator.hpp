//
//  Oscillator.hpp
//  bear
//
//  Created by Milan van der Meer on 25/07/16.
//
//

#ifndef BEAR_DSP_OSCILLATOR_HPP
#define BEAR_DSP_OSCILLATOR_HPP

#include "Math.hpp"

namespace bear::dsp
{
    //! Generate a bipolar saw wave given a normalised phase
    template <typename T>
    constexpr T generateSaw(T phase)
    {
        return 2 * bear::wrap(phase + 0.5, 0, 1) - 1;
    }
    
    //! Generate a bipolar square wave given a normalised phase
    template <typename T1, typename T2>
    constexpr T1 generateSquare(T1 phase, T2 pulseWidth)
    {
        return bear::wrap(phase, 0, 1) < pulseWidth ? 1 : -1;
    }
    
    //! Generate a unipolar square wave given a normalised phase
    template <typename T1, typename T2>
    constexpr T1 generateUnipolarSquare(T1 phase, T2 pulseWidth)
    {
        return bear::wrap(phase, 0, 1) < pulseWidth ? 1 : 0;
    }
    
    //! Generate a bipolar triangle wave given a normalised phase
    template <typename T>
    constexpr T generateTriangle(T phase)
    {
        return 2 * fabs(2 * bear::wrap(phase - 0.25, 0, 1) - 1) - 1;
    }
    
    //! Generate a unipolar triangle wave given a normalised phase
    template <typename T>
    constexpr T generateUnipolarTriangle(T phase)
    {
        return bear::wrap(phase, 0, 1) < 0.5 ? phase * 2 : (0.5 - (phase - 0.5)) * 2;
    }
}

#endif
