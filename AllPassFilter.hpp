//
//  AllPassFilter.hpp
//  wat
//
//  Created by Yuri Wilmering on 01/11/16.
//  Copyright © 2016 Yuri Wilmering. All rights reserved.
//

#ifndef GRIZZLY_ALL_PASS_FILTER_HPP
#define GRIZZLY_ALL_PASS_FILTER_HPP

#include <dsperados/math/constants.hpp>
#include <vector>

#include "Delay.hpp"

using namespace std;

namespace dsp
{
    template <class T>
    class AllPassFilter
    {
    public:
        AllPassFilter(std::size_t maxDelayTime) :
            delay(maxDelayTime)
        {

        }

        T process(const T& x, float delayTime, float gain = math::INVERSE_PHI<float>)
        {
            const auto w = gain * delay.read(delayTime) + x;
            const auto y = gain * w - delay.read(delayTime);

            delay.write(w);

            return y;
        }
        
        T operator()(const T& x, float delayTime, float gain = math::INVERSE_PHI<float>)
        {
            return process(x, delayTime, gain);
        }

        T getMaximumDelayTime()
        {
            return delay.getMaximumDelayTime();
        }

    private:
        Delay<T> delay;
    };
}


#endif
