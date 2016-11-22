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
    template <typename Iterator>
    inline static float centroid(Iterator begin, Iterator end)
    {
        // Accumulation of the numerator and the denominator
        typename Iterator::value_type numerator = 0;
        typename Iterator::value_type denominator = 0;
        
        // Multiply each value with its index number and accumulate
        for (auto index = 0; begin != end; ++begin)
        {
            numerator += *begin * index++;
            denominator += *begin;
        }
        
        return numerator / denominator;
    }
}

#endif
