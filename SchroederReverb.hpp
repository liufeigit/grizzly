//
//  SchroederReverb.hpp
//
//  Created by Yuri Wilmering on 07/11/16.
//  Copyright © 2016 Yuri Wilmering. All rights reserved.
//
#include <vector>

#include "AllPass.hpp"

using namespace bear;
using namespace std;

#ifndef BEAR_DSP_REVERB_HPP
#define BEAR_DSP_REVERB_HPP

namespace bear::dsp
{
    template <class T>
    class SchroederReverb : public dsp::Filter<T>
    {
    public:
        SchroederReverb(std::size_t delayTime = 2000, float gain = 0.7, int maxNrofFilters = 6) :
            delayTime(delayTime),
            gain(gain),
            maxNrofFilters(maxNrofFilters)
        {
            while (delayTime >= 1 && nrofFilters < maxNrofFilters)
            {
                allPass.emplace_back(delayTime, gain);
                delayTime /= 3;
                nrofFilters++;
            }
        }

        T process(const T& x) final override
        {
            _x = x;

            for (auto filter = 0; filter < nrofFilters; filter++)
            {
                y = allPass[filter](_x);
                _x = y;
            }

            return y;
        }

    public:
        float gain = 0;
        int maxNrofFilters = 0;
        int delayTime = 0;

    private:
        vector<dsp::AllPassFilter<float>> allPass;
        int nrofFilters = 0;
        float _x = 0;
        float y = 0;
    };
}


#endif
