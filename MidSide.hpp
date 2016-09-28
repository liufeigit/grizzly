//
//  MidSide.hpp
//  libbear
//
//  Created by Milan van der Meer on 07/03/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef BEAR_AUDIO_MID_SIDE_HPP
#define BEAR_AUDIO_MID_SIDE_HPP

#include <array>
#include <gsl/span.h>
#include <vector>

namespace bear::audio
{
    //! Convert a left and right stereo sample to mid-side
    template <class T>
    constexpr std::array<T, 2> stereo2ms(const T& left, const T& right)
    {
        return {{(left + right) * T(0.5), (left - right) * T(0.5)}};
    }
    
    //! Convert a left and right stereo sample to mid-side
    template <class T>
    constexpr std::array<T, 2> stereo2ms(gsl::span<const T, 2> stereoPair)
    {
        return stereo2ms(*stereoPair.begin(), *(stereoPair.begin()+1));
    }

    //! Convert mid-side sample to a stereo left-right
    template <class T>
    constexpr std::array<T, 2> ms2stereo(const T& mid, const T& side)
    {
        return {{mid + side, mid - side}};
    }
    
    //! Convert mid-side sample to a stereo left-right
    template <class T>
    constexpr std::array<T, 2> ms2stereo(gsl::span<const T, 2> msPair)
    {
        return ms2stereo(*msPair.begin(), *(msPair.begin()+1));
    }

    //! Convert a stereo signal to mid-side (in-place)
    void stereo2ms(gsl::span<const float> left, gsl::span<const float> right, gsl::span<float> mid, gsl::span<float> side);
    
    //! Convert a stereo signal to mid-side
    std::array<std::vector<float>, 2> stereo2ms(gsl::span<const float> left, gsl::span<const float> right);
    
    //! Convert a mid-side signal to stereo (in-place)
    void ms2stereo(gsl::span<const float> mid, gsl::span<const float> side, gsl::span<float> left, gsl::span<float> right);

    //! Convert a mid-side signal to stereo
    std::array<std::vector<float>, 2> ms2stereo(gsl::span<const float> mid, gsl::span<const float> side);
}

#endif /* BEAR_AUDIO_MID_SIDE_HPP */