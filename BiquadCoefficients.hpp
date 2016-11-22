//
//  BiquadCoefficients.hpp
//  bear-dsp
//
//  Created by Stijn Frishert on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_BIQUAD_COEFFICIENTS_HPP
#define GRIZZLY_BIQUAD_COEFFICIENTS_HPP

#include <cmath>
#include <unit/amplitude.hpp>
#include <unit/hertz.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Coefficients to a biquad
    /*! Credits to Robert Bristow-Johnson for providing the cooking formulas (see "Audio-EQ-cookbook") . Notice we use "a" for feed-forward and "b" for feed-back. */
    template <class T>
    struct BiquadCoefficients
    {
        //! The a0 feed-forward coefficient (gain)
        T a0 = 0;
        
        //! The a1 feed-forward coefficient
        T a1 = 0;
        
        //! The a2 feed-forward coefficient
        T a2 = 0;
        
        //! The b1 feed-back coefficient
        T b1 = 0;
        
        //! The b2 feed-back coefficient
        T b2 = 0;
    };
    
    //! Set biquad to through pass
    template <typename T>
    constexpr void biquadThroughPass(BiquadCoefficients<T>& coefficients)
    {
        coefficients.a0 = 1;
        coefficients.a1 = 0;
        coefficients.a2 = 0;
        coefficients.b1 = 0;
        coefficients.b2 = 0;
    }
    
    //! Set biquad to low pass filtering
    template <class T, class Q>
    constexpr void lowPass(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = ((1 - cosw) / 2) / b0;
        coefficients.a1 = (1 - cosw) / b0;
        coefficients.a2 = ((1 - cosw) / 2) / b0;
        
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
    //! Set biquad to high pass filtering
    template <class T, class Q>
    constexpr void highPass(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = ((1 + cosw) / 2) / b0;
        coefficients.a1 = (-(1 + cosw)) / b0;
        coefficients.a2 = ((1 + cosw) / 2) / b0;
        
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
    //! Set biquad to band pass filtering with a constant skirt gain
    template <class T, class Q>
    constexpr void bandPassConstantSkirt(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = (q * alpha) / b0;
        coefficients.a1 = 0;
        coefficients.a2 = (-q * alpha) / b0;
        
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
    //! Set biquad to band pass filtering with a constant peak gain
    template <class T, class Q>
    constexpr void bandPassConstantPeak(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = alpha / b0;
        coefficients.a1 = 0;
        coefficients.a2 = -alpha / b0;
        
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
    //! Set biquad to peak filtering with a constant peak gain
    template <class T, class Q, class Gain>
    constexpr void peakConstantSkirt(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q, const unit::decibel<Gain>& gain)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        const auto A = std::pow(10, gain / 40);
        
        const auto b0 = 1 + alpha / A;
        
        coefficients.a0 = (1 + alpha * A) / b0;
        coefficients.a1 = (-2 * cosw) / b0;
        coefficients.a2 = (1 - alpha * A) / b0;
        
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha / A) / b0;
    }
    
    //! Set biquad to peak filtering with a constant Q
    template <class T, class Q, class Gain>
    constexpr void peakConstantQ(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q, const unit::decibel<Gain>& gain)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        const auto A = std::pow(10, gain / 40);
        
        const auto b0 = 1 + alpha / A;

        // Negative peak
        if (A < 1)
        {
            coefficients.a0 = (1 + alpha) / b0;
            coefficients.a1 = (-2 * cosw) / b0;
            coefficients.a2 = (1 - alpha) / b0;
            
            coefficients.b1 = (-2 * cosw) / b0;
            coefficients.b2 = (1 - alpha / A) / b0;
        }
        // Positive peak
        else
        {
            coefficients.a0 = (1 + alpha * A) / b0;
            coefficients.a1 = (-2 * cosw) / b0;
            coefficients.a2 = (1 - alpha * A) / b0;
    
            coefficients.b1 = (-2 * cosw) / b0;
            coefficients.b2 = (1 - alpha) / b0;
        }
    }
    
    //! Set biquad to low shelf filtering
    template <class T, class Q, class Gain>
    constexpr void lowShelf(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q, const unit::decibel<Gain>& gain)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        const auto A = std::pow(10, gain / 40);
        
        const auto beta = sqrt(A)/q;
        const auto b0 = (A + 1) + (A - 1) * cosw + beta * sinw;
        
        coefficients.a0 = (A * ((A + 1) - (A - 1) * cosw + beta * sinw)) / b0;
        coefficients.a1 = (2 * A * ((A-1) - (A + 1) * cosw)) / b0;
        coefficients.a2 = (A * ((A + 1) - (A - 1) * cosw - beta * sinw)) / b0;
        
        coefficients.b1 = (-2 * ((A - 1) + (A + 1) * cosw)) / b0;
        coefficients.b2 = ((A + 1) + (A - 1) * cosw - beta * sinw) / b0;
    }
    
    //! Set biquad to high shelf filtering
    template <class T, class Q, class Gain>
    constexpr void highShelf(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q, const unit::decibel<Gain>& gain)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        const auto A = std::pow(10, gain / 40);
        
        const auto beta = sqrt(A)/q;
        const auto b0 = (A + 1) - (A - 1) * cosw + beta * sinw;
        
        coefficients.a0 = (A * ((A + 1) + (A-1) * cosw + beta * sinw)) / b0;
        coefficients.a1 = (-2 * A* ((A - 1) + (A + 1) * cosw)) / b0;
        coefficients.a2 = (A * ((A + 1) + (A - 1) * cosw - beta * sinw)) / b0;
        
        coefficients.b1 = (2 *((A - 1) - (A + 1) * cosw)) / b0;
        coefficients.b2 = ((A + 1) - (A - 1) * cosw - beta * sinw) / b0;
    }
    
    //! Set biquad to notch filtering
    template <class T, class Q>
    constexpr void notch(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = 1 / b0;
        coefficients.a1 = (-2 * cosw) / b0;
        coefficients.a2 = 1 / b0;
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
    //! Set biquad to all pass filtering
    template <class T, class Q>
    constexpr void allPass(BiquadCoefficients<T>& coefficients, unit::hertz<float> sampleRate, unit::hertz<float> cutOff, const Q& q)
    {
        const auto w = 2 * math::PI<float> * cutOff / sampleRate;
        const auto sinw = sin(w);
        const auto cosw = cos(w);
        const auto alpha = sinw / (2 * q);
        
        const auto b0 = 1 + alpha;
        
        coefficients.a0 = (1 - alpha) / b0;
        coefficients.a1 = (-2 * cosw) / b0;
        coefficients.a2 = (1 + alpha) / b0;
        coefficients.b1 = (-2 * cosw) / b0;
        coefficients.b2 = (1 - alpha) / b0;
    }
    
}

#endif
