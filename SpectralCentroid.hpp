//
//  SpectralCentroid.hpp
//  libbear
//
//  Created by Milan van der Meer on 13/11/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_AUDIO_SPECTRAL_CENTROID_HPP
#define BEAR_AUDIO_SPECTRAL_CENTROID_HPP

#include <unit/hertz.hpp>

#include "Centroid.hpp"

namespace bear::dsp
{
    template <class T>
    inline static unit::hertz<float> spectralCentroid(const std::vector<T>& magnitudes, unit::hertz<float> sampleRate)
    {
        return dsp::centroid(magnitudes) * sampleRate / magnitudes.size();
    }
}

#endif /* BEAR_AUDIO_SPECTRAL_CENTROID_HPP */
