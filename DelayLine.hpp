//
//  DelayLine.cpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_DELAY_LINE_HPP
#define GRIZZLY_DELAY_LINE_HPP


#include <cstddef>
#include <functional>
#include <numeric>
#include <vector>

#include "Delay.hpp"

namespace bear::dsp
{
    //! Delay interface
    template <class T>
    class DelayLine : public Delay<T>
    {
        
    public:
        //! Create a Delay interface
        DelayLine (const std::size_t maxDelay) :
            Delay<T> (maxDelay),
            delayTime (maxDelay)
        {
            
        }
        
        //! Read from the delay line
        template <class Interpolator = math::LinearInterpolation>
        T read(Interpolator interpolator = Interpolator()) const
        {
            return this->read(delayTime, interpolator);
        }
        
    public:
        //! The delay that will be used to read from the buffer
        /*! If delayTime > maxDelay given in the constructor, it will be clamped to the maximum delay time */
        double delayTime;

    private:
        using Delay<T>::read;
    };
}


#endif /* GRIZZLY_DELAY_LINE_HPP */
