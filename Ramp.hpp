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

#ifndef GRIZZLY_RAMP_HPP
#define GRIZZLY_RAMP_HPP

#include <functional>
#include <stdexcept>
#include <unit/hertz.hpp>

#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! A linearly as/descending ramp signal
    /*! Outputs the current state and increments (or decrements if negative) with each call.
        Output is then wrapped within a normalized range and can be used as an oscillator,
        or more typical, to drive the phase of another oscillator. */
    template <class T>
    class Ramp
    {
    public:
        //! Default constructor
        Ramp() = default;
        
        //! Construct the ramp with an initial phase
        Ramp(const T& phase)
        {
            setPhase(phase);
        }
        
        //! Set the phase for next callback, wrap if necessary
        void setPhase(const T& phase)
        {
            this->phase = math::wrap<T>(phase, 0, 1);
        }
        
        //! Increment the phase
        void increment(const T& increment)
        {
            phase += increment;
            if (phase >= 1 && end)
                end();
            
            phase = math::wrap<T>(phase, 0, 1);
        }
        
        //! Get phase
        T getPhase() const
        {
            return phase;
        }
        
    public:
        //! End function when ramp gets wrapped
        std::function<void(void)> end;

    private:
        //! Phase
        T phase = 0;
    };
}

#endif
