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
    class DelayLine
    {
        
    public:
        //! Create a Delay interface
        DelayLine (const std::size_t maxDelay) :
            delay(maxDelay),
            delayTime(maxDelay)
        {
            
        }
        
        //! Resize the delay buffer
        void resize (const std::size_t maxDelay)
        {
            delay.resize(maxDelay);
        }
        
        //! Get the  time of the delay
        const std::size_t getDelayTime () const
        {
            return delayTime;
        }
        
        //! Get the size of the delay buffer
        const std::size_t getMaxDelaySize () const
        {
            return delay.getMaxDelay();
        }
        
        //! Access the delay
        T delayRead()
        {
            return delay.read(delayTime);
        }
        
        //! Access the delay
        void delayWrite(T sample)
        {
            delay.write(sample);
        }
        
        
    public:
        //! The delay that will be used to read from the buffer
        /*! If delayTime > maxDelay given in the constructor, it will be clamped to the maximum delay time */
        double delayTime;
        
    private:
        //! The single delay buffer that will be used for all stages
        dsp::Delay<T> delay;
    };
}


#endif /* GRIZZLY_DELAY_LINE_HPP */
