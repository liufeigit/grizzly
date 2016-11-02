//
//  AllPass.hpp
//  wat
//
//  Created by Yuri Wilmering on 01/11/16.
//  Copyright © 2016 Yuri Wilmering. All rights reserved.
//
#include <vector>

#include "DelayLine.hpp"
#include "Filter.hpp"

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
            delayLine(maxDelayTime),
            gain(gain)
        {

        }

        T process(const T& x) final override
        {
            const auto w = gain * delayLine.read() + x;
            const auto y = gain * w - delayLine.read();

            delayLine.write(w);

            return y;
        }

        void setDelayTime(T delayTime)
        {
          delayLine.delayTime = delayTime;
        }

    public:
        float gain = 0;

    private:
        DelayLine<T> delayLine;
    };
}


#endif
