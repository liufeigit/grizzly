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

#ifndef GRIZZLY_COMB_FILTER_HPP
#define GRIZZLY_COMB_FILTER_HPP

#include <functional>

#include "Delay.hpp"

namespace dsp
{
    //! Feed-back Comb Filter
    template <class T>
    class FeedBackCombFilter
    {
    public:
        FeedBackCombFilter(const std::size_t maxDelay):
            delay(maxDelay)
        {
            
        }
        
        //! Process function, take an input
        T process(const T& x, float delayTime, float feedBack)
        {
            const auto d = delayTime > 0 ? delay.read(delayTime - 1) : 0;
            const auto y = x + feedBack * (postDelay ? postDelay(d) : d);
            
            delay.write(y);
            
            return y;
        }
        
        T operator()(const T& x, float delayTime, float feedBack)
        {
            return process(x, delayTime, feedBack);
        }

        void setMaxDelayTime(float maxDelayTime)
        {
            delay.resize(maxDelayTime);
        }
        
        float getMaxDelayTime()
        {
            return delay.getMaximumDelayTime ();
        }
        
    public:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
    private:
        Delay<T> delay;
    };
    
    //! Feed-forward Comb Filter
    template <class T>
    class FeedForwardCombFilter
    {
    public:
        FeedForwardCombFilter(const std::size_t maxDelay):
        	delay(maxDelay)
        {
            
        }
        
        //! Process function, take an input
        T process(const T& x, float delayTime, float feedForward)
        {
            delay.write(x);
            
            const auto d = delay.read(delayTime);
            const auto y = x + feedForward * (postDelay ? postDelay(d) : d);
            
            return y;
        }
        
        T operator()(const T& x, float delayTime, float feedForward)
        {
            return process(x, delayTime, feedForward);
        }
        
        void setMaxDelayTime(float maxDelayTime)
        {
            delay.resize(maxDelayTime);
        }
        
        float getMaxDelayTime()
        {
            return delay.getMaximumDelayTime ();
        }
        
        
    public:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
    private:
        Delay<T> delay;
    };
}
#endif /* GRIZZLY_COMB_FILTER_HPP */
