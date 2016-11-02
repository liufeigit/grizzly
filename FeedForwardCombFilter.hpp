//
//  FeedForwardCombFilter.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP
#define GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP

#include "DelayedFilter.hpp"

namespace bear::dsp
{
    //! Feedforward Comb Filter
    template <class T>
    class FeedForwardCombFilter : public dsp::DelayedFilter<T>
    {
    public:
        FeedForwardCombFilter (const std::size_t maxDelay, const double delayTime, double feedforward):
        DelayedFilter<T>(maxDelay,delayTime),
        feedforward(feedforward)
        {
            
        }
        
        
        //! Process function, take an input
        T process (const T& x) final override
        {
            
            this->delayWrite(x);
            const auto d = this->delayRead(this->delayTime);
            const auto y = x + this->feedforward * (this->postDelay ? this->postDelay(d) : d);
            
            return y;
        }
        
        void setFeedForward(double feedforward)
        {
            this->feedforward = feedforward;
        }
        
    public:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
        double feedforward;
        
    };
}
#endif /* GRIZZLY_FEED_FORWARD_COMB_FILTER_HPP */
