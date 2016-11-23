//
//  Delay.hpp
//  libbear
//
//  Created by Stijn on 02/05/2015.
//  Copyright (c) 2015 FrisHertz. All rights reserved.
//

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
            return static_cast<T>(math::interpolate(data.rbegin(), data.rend(), index, interpolator, math::ClampedAccess()));
        }
        
        //! Set the maximum delay
        void resize(std::size_t maximumDelayTime)
        {
            data.resize(maximumDelayTime + 1);
        }
        
        //! Return the maximum number of delay samples
        std::size_t getMaximumDelayTime() const { return data.size() - 1; }
        
    private:
        //! The data in the delay line
        CircularBuffer<T> data;
    };
}

#endif
