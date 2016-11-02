//
//  AllPass.hpp
//  wat
//
//  Created by Yuri Wilmering on 01/11/16.
//  Copyright © 2016 Yuri Wilmering. All rights reserved.
//

#include "grizzly-development/Delay.hpp"
#include "grizzly-development/Filter.hpp"
#include <vector>

using namespace bear;
using namespace std;

#ifndef BEAR_DSP_ALL_PASS_FILTER_HPP
#define BEAR_DSP_ALL_PASS_FILTER_HPP

namespace bear::dsp
{
    template <class T>
    class AllPassFilter : public dsp::Filter<T>
    {
    public:
        AllPassFilter(std::size_t maxDelayTime, float gain=0.618) :
            delay(maxDelayTime),
            delayTime(maxDelayTime),
            gain(gain)
        {
          
        }

        T process(const T& x) final override
        {
            const auto w = gain * delay.read(delayTime) + x;
            const auto y = gain * w - delay.read(delayTime);

            delay.write(w);

            return y;
        }

    public:
        float delayTime = 1;
        float gain = 0;

    private:
        Delay<T> delay;
    };
}


#endif
