//
//  FeedBackCombFilter.h
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_FEED_BACK_COMB_FILTER_HPP
#define GRIZZLY_FEED_BACK_COMB_FILTER_HPP

#include "DelayedFilter.hpp"

namespace bear::dsp
{
    //! Feedforward Comb Filter
    template <class T>
    class FeedBackCombFilter : public dsp::DelayedFilter<T>
    {
    public:
        FeedBackCombFilter (const std::size_t maxDelay, double feedBack = 0):
            DelayedFilter<T>(maxDelay),
            feedBack(feedBack)
        {
            
        }
        
        //! Process function, take an input
        T process (const T& x) final override
        {
            const auto d = this->delayRead();
            const auto y = x + feedBack * (this->postDelay ? this->postDelay(d) : d);
            
            this->delayWrite(y);
            
            return y;
        }
        
    public:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
        double feedBack = 0;
    };
}
#endif /* GRIZZLY_FEED_BACK_COMB_FILTER_HPP */
