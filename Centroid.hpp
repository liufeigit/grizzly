//
//  Centroid.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_CENTROID_HPP
#define BEAR_DSP_CENTROID_HPP

#include <gsl/span>

#include "Parallel.hpp"

namespace bear::dsp
{
    //! The centroid of a span of values
    /*! The centroid or 'center of gravity' is sum of values, weighted by its index, divided by the sum of values. */
    template <typename T, long N>
    inline static float centroid(gsl::span<T, N> values)
    {
        // Accumulator
        T indexWeightedSum = 0;
        
        // Multiply each value with its index number and accumulate
        for (auto index = 0; index < values.size(); ++index)
        {
            indexWeightedSum += index * values[index];
        }
        
        return indexWeightedSum / sum<T>(values);
    }
}

#endif /* BEAR_DSP_CENTROID_HPP */