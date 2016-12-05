//
//  AnalogOnePoleFilter.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 28/09/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef GRIZZLY_ANALOGONEPOLEFILTER_HPP
#define GRIZZLY_ANALOGONEPOLEFILTER_HPP

#include <experimental/optional>
#include <unit/hertz.hpp>

#include <dsperados/math/utility.hpp>

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
            auto unresolvedCutOffGain = std::tan(PI<T> * cutOff / sampleRate);
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
        }
        
        //! Get the integrator state, useful when creating more complex filter structures
        auto getState() const
        {
            return integratorState;
        }
        
        //! Process and return with optional distortion for non-linear processing
        T process(const T& x, std::experimental::optional<float> distortionFactor)
        {
            auto integratorInput = (x - integratorState) * cutOffGain;
            
            auto y = integratorInput + integratorState;
            
            integratorState = y + integratorInput;
            
            if (distortionFactor)
                integratorState = tanh(integratorState * *distortionFactor);
            
            return y;
        }
        
        //! Process and return the output with optional distortion
        T operator()(const T& x)
        {
            return process(x);
        }
        
    private:
        //! Filter gain factor with resolved zero delay feedback
        T cutOffGain = 0;
        
        //! Integrator state
        T integratorState = 0;
    };
}

#endif
