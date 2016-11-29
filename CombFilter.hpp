//
//  CombFilter.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_COMB_FILTER_HPP
#define GRIZZLY_COMB_FILTER_HPP

#include "Delay.hpp"
#include <Functional>

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
