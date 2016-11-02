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
        FeedBackCombFilter (const std::size_t maxDelay, const double delayTime, double feedback):
        DelayedFilter<T>(maxDelay,delayTime),
        feedback(feedback)
        {
            
        }
        
        
        //! Process function, take an input
        T process (const T& x) final override
        {
            const auto d = this->delay.read(this->delayTime);
            const auto y = x + this->feedback * (this->postDelay ? this->postDelay(d) : d);
            this->delay.write(y);
            return y;
        }
        
        void setFeedBack(double feedback)
        {
            this->feedback = feedback;
        }
        
    private:
        double feedback;
        
    };
}
#endif /* GRIZZLY_FEED_BACK_COMB_FILTER_HPP */
