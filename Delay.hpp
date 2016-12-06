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

#ifndef GRIZZLY_DELAY_HPP
#define GRIZZLY_DELAY_HPP

#include <dsperados/math/interpolation.hpp>

#include "CircularBuffer.hpp"

namespace dsp
{
    //! A simple sample delay object
    /*! Delay based on a circular buffer, capable of interpolation */
    template <class T>
    class Delay
    {
    public:
        //! Construct by feeding the maximum delay size
        Delay(std::size_t maximumDelayTime) :
            data(maximumDelayTime + 1)
        {
            
        }
        
        //! Push a sample in the delay line
        template <class... Args>
        void write(Args&&... args)
        {
            data.emplace_back(std::forward<Args&&>(args)...);
        }
        
        //! Read from the delay line
        template <class Index, class Interpolator = math::LinearInterpolation>
        T read(Index index, Interpolator interpolator = Interpolator()) const
        {
            return interpolate(data.rbegin(), data.rend(), index, interpolator, math::ClampedAccess());
        }
        
        //! Set the maximum delay
        void resize(std::size_t maximumDelayTime)
        {
            data.resize_front(maximumDelayTime + 1);
        }
        
        //! Return the maximum number of delay samples
        std::size_t getMaximumDelayTime() const { return data.size() - 1; }
        
    private:
        //! The data in the delay line
        CircularBuffer<T> data;
    };
}

#endif
