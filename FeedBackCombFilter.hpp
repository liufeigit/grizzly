//
//  FeedBackCombFilter.h
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FEED_BACK_COMB_FILTER_HPP
#define GRIZZLY_FEED_BACK_COMB_FILTER_HPP

#include "DelayLine.hpp"
#include "Filter.hpp"

namespace dsp
{
    //! Feedforward Comb Filter
    template <class T>
    class FeedBackCombFilter : public dsp::Filter<T>
    {
    public:
        FeedBackCombFilter (const std::size_t maxDelay, double feedBack = 0):
            delayLine(maxDelay),
            feedBack(feedBack)
        {
            
        }
        
        //! Process function, take an input
        T process (const T& x) final override
        {
            const auto d = delayLine.read();
            const auto y = x + feedBack * (postDelay ? postDelay(d) : d);
            
            delayLine.write(y);
            
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
        
        double feedBack = 0;
        
    private:
        DelayLine<T> delayLine;
        
    };
}
#endif /* GRIZZLY_FEED_BACK_COMB_FILTER_HPP */
