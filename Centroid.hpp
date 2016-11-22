//
//  Centroid.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_CENTROID_HPP
#define GRIZZLY_CENTROID_HPP

namespace dsp
{
    //! The centroid of a span of values
    /*! The centroid or 'center of gravity' is sum of values, weighted by its index, divided by the sum of values. */
    template <typename T>
    inline static float centroid(const std::vector<T>& values)
    {
        // Accumulation of the numerator and the denominator
        T numerator = 0;
        T denominator = 0;
        
        // Multiply each value with its index number and accumulate
        for (auto index = 0; index < values.size(); ++index)
        {
            numerator += index * values[index];
            denominator += values[index];
        }
        
        return numerator / denominator;
    }
}

#endif
