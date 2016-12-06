//
//  AnalogOnePoleFilter.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 28/09/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef GRIZZLY_ANALOGONEPOLEFILTER_HPP
#define GRIZZLY_ANALOGONEPOLEFILTER_HPP

#include <cmath>
#include <experimental/optional>
#include <unit/hertz.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Topology preserving one pole filter with resolved zero feedback delay
    /*! See "The Art Of VA Filter Design" by Vadim Zavalishin. */
    template <class T>
    class AnalogOnePoleFilter
    {
    public:
        //! Set cut-off
        void setCutOff(unit::hertz<float> cutOff, unit::hertz<float> sampleRate)
        {
            auto unresolvedCutOffGain = std::tan(math::PI<T> * cutOff / sampleRate);
            cutOffGain = unresolvedCutOffGain / (1.0 + unresolvedCutOffGain);
        }
        
        //! Set cutoff gain directly, useful when creating more complex filter structures
        void setCutOffGain(T cutOffGain)
        {
            this->cutOffGain = cutOffGain;
        }
        
        //! Set filter state
        void setState(T state)
        {
            integratorState = state;
            lowPassOutputState = state;
            highPassOutputState = 0;
        }
        
        //! Reset the filter
        void reset()
        {
            setState(0);
        }
        
        //! Get the integrator state, useful when creating more complex filter structures
        T getIntegratorState() const
        {
            return integratorState;
        }
        
        //! Process with optional distortion for non-linear processing
        void increment(const T& x, std::experimental::optional<float> distortionFactor)
        {
            auto integratorInput = (x - integratorState) * cutOffGain;
            
            lowPassOutputState = integratorInput + integratorState;
            
            highPassOutputState = x - lowPassOutputState;
            
            integratorState = lowPassOutputState + integratorInput;
            
            if (distortionFactor)
                integratorState = tanh(integratorState * *distortionFactor);
        }
        
        //! Read the low-pass output
        T readLowPass() const
        {
            return lowPassOutputState;
        }
        
        //! Read the high-pass output
        T readHighPass() const
        {
            return highPassOutputState;
        }
        
    private:
        //! Filter gain factor with resolved zero delay feedback
        T cutOffGain = 0;
        
        //! Integrator state
        T integratorState = 0;
        
        //! Low-pass output state
        T lowPassOutputState = 0;
        
        //! Low-pass output state
        T highPassOutputState = 0;
    };
}

#endif
