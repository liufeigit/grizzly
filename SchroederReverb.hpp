//
//  SchroederReverb.hpp
//
//  Created by Yuri Wilmering on 07/11/16.
//  Copyright © 2016 Yuri Wilmering. All rights reserved.
//

#ifndef BEAR_DSP_REVERB_HPP
#define BEAR_DSP_REVERB_HPP

#include <vector>

#include "AllPass.hpp"
#include "FeedBackCombFilter.hpp"

using namespace bear;
using namespace std;

#include <iostream>

// Work to be done: gain factors of combfilters
namespace bear::dsp
{
    template <class T>
    class SchroederReverb : public dsp::Filter<T>
    {
    public:
        SchroederReverb(std::size_t delayTime = 5000, float gain = 0.5) :
            combDelayTime(delayTime),
            allPassDelayTime(delayTime/10),
            nrofCombFilters(4),
            nrofAllPassFilters(2),
            gain(gain)
        {
            // initialize comb filters
            for (auto combFilter = 0; combFilter < nrofCombFilters; combFilter++)
            {
                combFilters.emplace_back(combDelayTime, gain);

                // make sure the ratio of smallest and largest delay is 1:1.5
                static auto ratioFrac = 0.5 / nrofCombFilters;
                static auto ratio = 1.f;
                static auto _combDelayTime = combDelayTime;
                ratio += ratioFrac;

                combDelayTime = _combDelayTime * ratio;

                // make sure successive delaytimes have no common factors
                while (combDelayTime % 2 == 0 || combDelayTime % 3 == 0)
                {
                    combDelayTime += 1;
                }
            }

            // initialize allpass filters
            while (allPassDelayTime >= 1 && allPassFilters.size() < nrofAllPassFilters)
            {
                allPassFilters.emplace_back(allPassDelayTime, gain);
                allPassDelayTime /= 3;

                // make sure successive delaytimes have no common factors
                while (allPassDelayTime % 2 == 0 || allPassDelayTime % 3 == 0)
                {
                    allPassDelayTime += 1;
                }
            }
        }

        T process(const T& x) final override
        {
            auto y = 0.0f;

            for (auto& combFilter : combFilters)
            {
                y += combFilter(x);
            }

            for (auto& allPassFilter : allPassFilters)
                 y = allPassFilter(y);

            return y;
        }

        auto getFilterCount() const { return allPassFilters.size(); }

    public:
        int combDelayTime = 0;
        int allPassDelayTime = 0;
        int nrofCombFilters = 0;
        int nrofAllPassFilters = 0;
        float gain = 0;

    private:
        std::vector<dsp::FeedBackCombFilter<float>> combFilters;
        std::vector<dsp::AllPassFilter<float>> allPassFilters;
    };
}


#endif
