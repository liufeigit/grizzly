//
//  MidSide.cpp
//  libbear
//
//  Created by Stijn Frishert on 07/03/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#include "MidSide.hpp"

using namespace std;

namespace bear::audio
{
    void stereo2ms(const vector<float>& left, const vector<float>& right, vector<float>& mid, vector<float>& side)
    {
        const auto frameCount = min({left.size(), right.size(), mid.size(), side.size()});

        for (auto i = 0; i < frameCount; ++i)
        {
            mid[i] = (left[i] + right[i]) / 2;
            side[i] = (left[i] - right[i]) / 2;
        }
    }

    array<vector<float>, 2> stereo2ms(const vector<float>& left, const vector<float>& right)
    {
        const auto frameCount = min(left.size(), right.size());

        array<vector<float>, 2> result;
        result[0] = vector<float>(frameCount); // mid
        result[1] = vector<float>(frameCount); // side

        stereo2ms(left, right, result[0], result[1]);

        return result;
    }

    void ms2stereo(const vector<float>& mid, const vector<float>& side, vector<float>& left, vector<float>& right)
    {
        const auto frameCount = min({mid.size(), side.size(), left.size(), right.size()});

        for (auto i = 0; i < frameCount; ++i)
        {
            left[i] = mid[i] + side[i];
            right[i] = mid[i] - side[i];
        }
    }

    array<vector<float>, 2> ms2stereo(const vector<float>& mid, const vector<float>& side)
    {
        const auto frameCount = min(mid.size(), side.size());

        array<vector<float>, 2> result;
        result[0] = vector<float>(frameCount); // left
        result[1] = vector<float>(frameCount); // right

        ms2stereo(mid, side, result[0], result[1]);

        return result;
    }
}
