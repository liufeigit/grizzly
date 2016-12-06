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

#ifndef GRIZZLY_ANALOG_ONE_POLE_FILTER_HPP
#define GRIZZLY_ANALOG_ONE_POLE_FILTER_HPP

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
        //! Process with optional distortion for non-linear processing
        void write(const T& x, std::experimental::optional<float> distortionFactor = std::experimental::nullopt)
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
