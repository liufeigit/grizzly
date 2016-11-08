//
//  HighFrequencyContent.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_AUDIO_HIGH_FREQUENCY_CONTENT_HPP
#define BEAR_AUDIO_HIGH_FREQUENCY_CONTENT_HPP

#include <unit/hertz.hpp>

#include "../Dsp/WeightedSum.hpp"

namespace bear::audio
{
    //! Retrieve the high-frequency content according to Brossier
    template <class T>
    constexpr auto highFrequencyContentBrossier(const std::vector<T>& magnitudes, const unit::hertz<float>& sampleRate)
    {
        return dsp::indexWeightedSum<T>(magnitudes) * sampleRate / magnitudes.size();
    }
    
    //! Retrieve the high-frequency content according to Masri
    template <class T>
    constexpr auto highFrequencyContentMasri(const std::vector<T>& magnitudes, const unit::hertz<float>& sampleRate)
    {
        T acc = 0;
        for (auto i = 0; i < magnitudes.size(); ++i)
            acc += i * magnitudes[i] * magnitudes[i];
        
        return acc * sampleRate / magnitudes.size();
    }
    
    //! Retrieve the high-frequency content according to Masri
    template <class T>
    constexpr auto highFrequencyContentJensen(const std::vector<T>& magnitudes, const unit::hertz<float>& sampleRate)
    {
        T acc = 0;
        for (auto i = 0; i < magnitudes.size(); ++i)
            acc += i * i * magnitudes[i];
        
        return acc * sampleRate / magnitudes.size();
    }
}

#endif
