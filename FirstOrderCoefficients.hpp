/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
        const auto w = math::TWO_PI<double> * cutOff.value / sampleRate.value;
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = 1.0 - coefficients.b1;
        coefficients.a1 = 0;
    }
    
    //! Set filter to low pass filtering using one pole, given a samplerate, time and and a time constant factor.
    /*! @param timeConstantFactor: Affects the actual time. A factor of 1 means a step response where the output reaches to ~63% in the given time. A factor of 5 reaches to ~99%. */
    template <typename T>
    constexpr void lowPassOnePole(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::second<float> time, float timeConstantFactor = 5.f)
    {
        if (time.value <= 0)
            return throughPass(coefficients);
            
        const auto w = timeConstantFactor / (time.value * sampleRate.value);
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = 1.0 - coefficients.b1;
        coefficients.a1 = 0;
    }
    
    //! Set filter to low pass filtering using one pole and one zero, given a samplerate and a cutoff
    template <typename T>
    constexpr void lowPassOnePoleZero(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff)
    {
        const auto w = math::TWO_PI<double> * cutOff.value / sampleRate.value;
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = (1.0 - coefficients.b1) / 2;
        coefficients.a1 = coefficients.a0;
    }
    
    //! Set filter to high pass filtering using one pole and one zero, given a samplerate and a cutoff
    template <typename T>
    constexpr void highPassOnePoleZero(FirstOrderCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff)
    {
        const auto w = math::TWO_PI<double> * cutOff.value / sampleRate.value;
        
        coefficients.b1 = exp(-w);
        coefficients.a0 = (1 + coefficients.b1) / 2;
        coefficients.a1 = -coefficients.a0;
    }
}

#endif /* GRIZZLY_FIRST_ORDER_COEFFICIENTS_HPP */
