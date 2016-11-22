//
//  FeedForwardCombFilter.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP
#define GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP

#include "DelayLine.hpp"
#include "Filter.hpp"

namespace dsp
{
    //! Feedforward Comb Filter
    template <class T>
    class FeedForwardCombFilter : public dsp::Filter<T>
    {
    public:
        FeedForwardCombFilter (const std::size_t maxDelay, double feedForward = 0):
            delayLine(maxDelay),
            feedForward(feedForward)
        {
            
        }
        
        //! Process function, take an input
        T process (const T& x) final override
        {
            delayLine.write(x);
            
            const auto d = delayLine.read();
            const auto y = x + feedForward * (postDelay ? postDelay(d) : d);
            
            return y;
        }
        
        void setDelayTime(float delayTime)
        {
            delayLine.delayTime = delayTime;
        }
        
        void setMaxDelayTime(float maxDelayTime)
        {
            delayLine.resize(maxDelayTime);
        }
        
        
    public:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
        double feedForward;
        
    private:
        DelayLine<T> delayLine;
        
    };
}
#endif /* GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP */
