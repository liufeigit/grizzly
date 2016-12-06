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

#ifndef GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP
#define GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP

#include <cmath>
#include <unit/hertz.hpp>
#include <unit/radian.hpp>

#include <dsperados/math/constants.hpp>

namespace dsp
{
    //! Sine approximation using Gordon-Smith
    template <class T>
    class GordonSmithOscillator
    {
    public:
        //! Construct the oscillator
        constexpr GordonSmithOscillator() = default;
        
        //! Construct the oscillator
        constexpr GordonSmithOscillator(unit::radian<float> angle)
        {
            setAngle(angle);
        }
        
        //! Compute the next sample
        constexpr T process()
        {
            yq -= epsilon * y;
            
            return y += epsilon * yq;
        }
        
        //! Compute the next sample
        constexpr T operator()()
        {
            return process();
        }
        
        //! Change the angle the oscillator should increment every process
        constexpr void setAngle(unit::radian<float> angle)
        {
            epsilon = 2 * sin(angle / 2);
            y = sin(-angle);
            yq = cos(-angle);
        }
        
    private:
        //! The last calculated y
        T y = 0;
        
        //! The last calculaced quadratic y (90 degrees from y)
        T yq = 0;
        
        //! The Gordon-Smith epsilon = 2 * sin(theta / 2)
        double epsilon = 0;
        
        //! The frequency of the sine
        unit::hertz<float> frequency;
    };
}

#endif /* GRIZZLY_GORDON_SMITH_OSCILLATOR_HPP */
