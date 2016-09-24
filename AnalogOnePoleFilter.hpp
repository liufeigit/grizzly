//
//  AnalogOnePoleFilter.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 28/09/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef BEAR_DSP_ANALOGONEPOLEFILTER_HPP
#define BEAR_DSP_ANALOGONEPOLEFILTER_HPP

#include <experimental/optional>
#include <unit/hertz.hpp>

#include "Math.hpp"

namespace bear::dsp
{
    //! Topology preserving one pole filter with resolved zero feedback delay
    /*! See "The Art Of VA Filter Design" by Vadim Zavalishin. */
    template <class T1, class T2 = double>
    class AnalogOnePoleFilter
    {
    public:
        //! Set filter coefficients
        void setCoefficients(unit::hertz<T2> cutOff, unit::hertz<T2> sampleRate)
        {
            auto integratorGainFactor = std::tan(PI<T2> * cutOff / sampleRate);
            gainFactor = integratorGainFactor / (1.0 + integratorGainFactor);
        }
        
        //! Use an external gain factor, useful for creating more complex filter structures
        void setGainFactor(T2 gainFactor)
        {
            this->gainFactor = gainFactor;
        }
        
        //! Set filter state
        void setState(T2 state)
        {
            integratorState = state;
        }
        
        //! Get the integrator state, useful for creating more complex filter structures
        auto getState() const
        {
            return integratorState;
        }
        
        //! Set optional distortion factor for non-linear processing
        void setDistortion(std::experimental::optional<T2> factor)
        {
            distortionFactor = factor;
        }
        
        //! Process and return the output with optional distortion
        T1 operator()(const T1& x)
        {
            auto integratorInput = (x - integratorState) * gainFactor;
            
            auto y = integratorInput + integratorState;
            
            integratorState = y + integratorInput;
            if (distortionFactor)
                integratorState = tanh(integratorState * *distortionFactor);
            
            return y;
        }
        
    private:
        //! Filter gain factor with resolved zero delay feedback
        T2 gainFactor = 0;
        
        //! Integrator state
        T2 integratorState = 0;
        
        //! Optinal distortion factor for non-linear processing
        std::experimental::optional<T2> distortionFactor;
        
    };
}

#endif
