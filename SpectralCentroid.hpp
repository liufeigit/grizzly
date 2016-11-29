//
//  SpectralCentroid.hpp
//  libbear
//
//  Created by Milan van der Meer on 13/11/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_AUDIO_SPECTRAL_CENTROID_HPP
#define BEAR_AUDIO_SPECTRAL_CENTROID_HPP

//#include <dsperados/math/statistics.hpp>
#include <unit/hertz.hpp>

namespace dsp
{
    //! The centroid of a range of values
    /*! The centroid or 'center of gravity' is sum of values, weighted by its index, divided by the sum of values. */
    template <typename Iterator>
    inline static float spectralCentroidBin(Iterator begin, Iterator end)
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

#endif /* BEAR_AUDIO_SPECTRAL_CENTROID_HPP */
