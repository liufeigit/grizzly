//
//  Centroid.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_CENTROID_HPP
#define BEAR_DSP_CENTROID_HPP

#include <gsl/gsl.h>

#include "WeightedSum.hpp"

namespace bear::dsp
{
    //! The centroid of a span of values
    template <typename T, long N>
    inline static float centroid(gsl::span<T, N> values)
    {
        return indexWeightedSum<T>(values) / sum(values);
    }
}

#endif /* BEAR_DSP_CENTROID_HPP */
