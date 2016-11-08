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

using namespace bear;
using namespace std;

#include <iostream>

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
            while (delayTime >= 1 && allPass.size() < maxNrofFilters)
            {
                allPass.emplace_back(delayTime, gain);
                delayTime /= 3;
            }
        }

        T process(const T& x) final override
        {
            auto y = x;

            for (auto& filter : allPass)
                y = filter(y);

            return y;
        }

        auto getFilterCount() const { return allPass.size(); }

    public:
        float gain = 0;
        int maxNrofFilters = 0;
        int delayTime = 0;

    private:
        std::vector<dsp::AllPassFilter<float>> allPass;
    };
}


#endif
