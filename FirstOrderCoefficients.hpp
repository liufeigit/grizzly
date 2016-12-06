//
//  FirstOrderCoefficients.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FIRST_ORDER_COEFFICIENTS_HPP
#define GRIZZLY_FIRST_ORDER_COEFFICIENTS_HPP

#include <cmath>
#include <unit/hertz.hpp>
#include <unit/time.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Coefficients for a first-order, one-pole/one-zero filter
    template <class T>
    struct FirstOrderCoefficients
    {
        //! The a0 feed-forward coefficient (gain)
        T a0 = 0;
        
        //! The a1 feed-forward coefficient
        T a1 = 0;
        
        //! The b1 feed-back coefficient
        T b1 = 0;
    };
    
    //! Set filter to through pass
    template <typename T>
    constexpr void throughPass(FirstOrderCoefficients<T>& coefficients)
    {
        coefficients.a0 = 1;
        coefficients.a1 = 0;
        coefficients.b1 = 0;
    }
    
    //! Set filter to low pass filtering using one pole, given a samplerate and a cutoff
    template <typename T>
    constexpr void lowPassOnePole(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff)
    {
        const auto w = math::TWO_PI<double> * cutOff / static_cast<long double>(sampleRate);
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = 1.0 - coefficients.b1;
        coefficients.a1 = 0;
    }
    
    //! Set filter to low pass filtering using one pole, given a samplerate, time and and a time constant factor.
    /*! @param timeConstantFactor: Affects the actual time. A factor of 1 means a step response where the output reaches to ~63% in the given time. A factor of 5 reaches to ~99%. */
    template <typename T>
    constexpr void lowPassOnePole(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::second<float> time, float timeConstantFactor = 5.f)
    {
        if (time <= 0)
            return throughPass(coefficients);
            
        const auto w = timeConstantFactor / static_cast<long double>(time * sampleRate);
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = 1.0 - coefficients.b1;
        coefficients.a1 = 0;
    }
    
    //! Set filter to low pass filtering using one pole and one zero, given a samplerate and a cutoff
    template <typename T>
    constexpr void lowPassOnePoleZero(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff)
    {
        const auto w = math::TWO_PI<double> * cutOff / static_cast<long double>(sampleRate);
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = (1.0 - coefficients.b1) / 2;
        coefficients.a1 = coefficients.a0;
    }
    
    //! Set filter to high pass filtering using one pole and one zero, given a samplerate and a cutoff
    template <typename T>
    constexpr void highPassOnePoleZero(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff)
    {
        const auto w = math::TWO_PI<double> * cutOff / static_cast<long double>(sampleRate);
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = (1 + coefficients.b1) / 2;
        coefficients.a1 = -coefficients.a0;
    }
}

#endif /* GRIZZLY_FIRST_ORDER_COEFFICIENTS_HPP */
