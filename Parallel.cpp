//
//  Parallel.cpp
//  libbear
//
//  Created by Stijn on 05/11/2015.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#include <cassert>

#include "Parallel.hpp"

using namespace std;

namespace bear
{
    void normalizeSum(std::vector<float>& x, double sum)
    {
        double gainFactor = 1.0 / sum;
        double accumulator = 0;

        for (auto& value : x)
            accumulator += value;

        if (accumulator) // OF RUN TIME ERROR?!
            for (auto& value : x)
                value /= (accumulator * gainFactor);
    }

    void normalizeBiDirectional(std::vector<float>& x, double peakMaximum)
    {
        double highest = 0;

        for (auto& value : x)
            if (fabs(value) > highest) highest = fabs(value);

        double gainFactor = fabs(peakMaximum) / highest;

        for (auto& value : x)
            value *= gainFactor;
    }

    void normalizeUniDirectional(std::vector<float>& x, double peakMaximum)
    {
        if (peakMaximum <= 0)
            throw std::runtime_error("Peak maximum has to be higher than zero. Use normalizeBilateral instead?");

        double highest = 0;

        if (!highest)
            throw std::runtime_error("There has to be at least one point above zero. Use normalizeBilateral instead?");

        for (auto& value : x)
            if (value > highest) highest = value;

        double gainFactor = peakMaximum / highest;

        for (auto& value : x)
            value *= gainFactor;
    }

}
