//
//  WeightedSum.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_WEIGHTED_SUM_HPP
#define BEAR_DSP_WEIGHTED_SUM_HPP

#include <gsl/gsl.h>

namespace bear::dsp
{
    //! Multiply each element in a span by its index
    template <class T>
    constexpr auto indexWeightedSum(gsl::span<const T> input)
    {
        T acc = 0;
        
        // Start at index 1, because the first element will be multiplied with 0 anyway
        for (auto i = 1; i < input.size(); ++i)
            acc += i * input[i];
        
        return acc;
    }
}

#endif /* BEAR_DSP_WEIGHTED_SUM_HPP */
