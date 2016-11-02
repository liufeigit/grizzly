//
//  CombFilter.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 01/11/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_COMB_FILTER_HPP
#define GRIZZLY_COMB_FILTER_HPP


#include <cstddef>
#include <functional>
#include <numeric>
#include <vector>

#include "Delay.hpp"
#include "Filter.hpp"


namespace bear::dsp
{
    //! Comb Filter
    template <class T>
    class CombFilter : public dsp::Filter<T>
    {
        
    public:
        //! Create a CombFilter
        CombFilter (const std::size_t maxDelay, const double delayTime) :
        maxDelay(maxDelay),
        delay(maxDelay),
        delayTime(delayTime)
        {
            
        }
        
        //! Resize the delay buffer
        void resize (const std::size_t maxDelay)
        {
            this->maxDelay = maxDelay;
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
            return maxDelay;
        }
        
    protected:
        //! PostDelay function
        std::function<T(const T&)> postDelay;
        
        double delayTime;
        
        std::size_t maxDelay;
        
        //! The single delay buffer that will be used for all stages
        dsp::Delay<T> delay;
    };
}


#endif /* GRIZZLY_COMB_FILTER_HPP */
