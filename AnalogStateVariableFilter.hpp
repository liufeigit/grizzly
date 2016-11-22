//
//  AnalogStateVariableFilter.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 28/09/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef GRIZZLY_ANALOG_STATE_VARIABLE_FILTER_HPP
#define GRIZZLY_ANALOG_STATE_VARIABLE_FILTER_HPP

#include <array>
#include <cmath>
#include <experimental/optional>
#include <unit/hertz.hpp>

#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! Topology preserving state variable filter with resolved zero feedback delay
    /*! See "Designing software synthesizer plug-ins in c++" by Will Pirkle.
        See "The Art Of VA Filter Design" by Vadim Zavalishin. */
    template <class T1, class T2 = double>
    class AnalogStateVariableFilter
    {
    public:
        //! Set coefficients
        void setCoefficients(unit::hertz<T2> cutOff, const T2& q, unit::hertz<T2> sampleRate)
        {
            dampingFactor = 1.0 / (2.0 * q);
            integratorGainFactor = std::tan(PI<T2> * cutOff / sampleRate);
            gainFactor = 1.0 / (1.0 + 2.0 * dampingFactor * integratorGainFactor + integratorGainFactor * integratorGainFactor);
        }
        
        //! Set filter state
        void setState(const T2& state)
        {
            integratorState1 = integratorState2 = state;
        }
        
        //! Set optional distortion factor for non-linear processing
        void setDistortion(std::experimental::optional<T2> factor)
        {
            distortionFactor = factor;
        }
        
        //! Process and return low- band- and high-pass with optional distortion
        std::array<T1, 3> operator()(const T1& x)
        {
            auto highPass = (x - 2.0 * dampingFactor * integratorState1 - integratorGainFactor * integratorState1 - integratorState2) * gainFactor;
            
            auto bandPass = integratorGainFactor * highPass + integratorState1;
            
            if (distortionFactor)
                bandPass = std::tanh(bandPass * *distortionFactor);
            
            auto lowPass = integratorGainFactor * bandPass + integratorState2;
            
            integratorState1 = integratorGainFactor * highPass + bandPass;
            integratorState2 = integratorGainFactor * bandPass + lowPass;
            
            return {lowPass, bandPass, highPass};
        }
        
    private:
        //! Damping factor, related to q
        T2 dampingFactor = 1;
        
        //! Integrator gain factor
        T2 integratorGainFactor = 0;
        
        //! Filter gain factor with resolved zero delay feedback
        T2 gainFactor = 0;
        
        //! Integrator state 1
        T2 integratorState1 = 0;
        
        //! Integrator state 2
        T2 integratorState2 = 0;
        
        //! Optinal distortion factor for non-linear processing
        std::experimental::optional<T2> distortionFactor;
        
    };
}

#endif
