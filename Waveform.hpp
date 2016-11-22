//
//  Waveform.hpp
//  bear
//
//  Created by Milan van der Meer on 25/07/16.
//
//

#ifndef GRIZZLY_WAVEFORM_HPP
#define GRIZZLY_WAVEFORM_HPP

#include <cmath>
#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! Generate a bipolar sine wave given a normalised phase
    template <typename T>
    constexpr T generateSine(T phase)
    {
        return std::sin(math::TWO_PI<T> * phase);
    }

    //! Generate a unipolar sine wave given a normalised phase
    template <typename T>
    constexpr T generateUnipolarSine(T phase)
    {
        return std::sin(math::TWO_PI<T> * phase) * 0.5 + 0.5;
    }
    
    //! Generate a bipolar saw wave given a normalised phase
    template <typename T>
    constexpr T generateSaw(T phase)
    {
        return 2 * math::wrap<T>(phase + 0.5, 0, 1) - 1;
    }
    
    //! Generate a bipolar square wave given a normalised phase
    template <typename T1, typename T2>
    constexpr T1 generateSquare(T1 phase, T2 pulseWidth)
    {
        return math::wrap<T1>(phase, 0, 1) < pulseWidth ? 1 : -1;
    }
    
    //! Generate a unipolar square wave given a normalised phase
    template <typename T1, typename T2>
    constexpr T1 generateUnipolarSquare(T1 phase, T2 pulseWidth)
    {
        return math::wrap<T1>(phase, 0, 1) < pulseWidth ? 1 : 0;
    }
    
    //! Generate a bipolar triangle wave given a normalised phase
    template <typename T>
    constexpr T generateTriangle(T phase)
    {
        return 2 * std::fabs(2 * math::wrap<T>(phase - 0.25, 0, 1) - 1) - 1;
    }
    
    //! Generate a unipolar triangle wave given a normalised phase
    template <typename T>
    constexpr T generateUnipolarTriangle(T phase)
    {
        return math::wrap<T>(phase, 0, 1) < 0.5 ? phase * 2 : (0.5 - (phase - 0.5)) * 2;
    }
}

#endif
