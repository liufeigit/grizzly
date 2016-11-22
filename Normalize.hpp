//
//  Normalize.hpp
//  grizzly
//
//  Created by Milan van der Meer on 16/11/16.
//
//

#ifndef GRIZZLY_NORMALIZE_HPP
#define GRIZZLY_NORMALIZE_HPP

#include <algorithm>
#include <numeric>
#include <stdexcept>

#include <dsperados/math/analysis.hpp>

namespace dsp
{
    //! Normalize an area so the integral of the signal equals one
    template <class InputIterator, class OutputIterator>
    void normalizeArea(InputIterator inBegin, InputIterator inEnd, OutputIterator outBegin)
    {
        auto integral = std::accumulate(inBegin, inEnd, typename InputIterator::value_type{0});
        
        if (!integral)
            throw std::runtime_error("area equals zero");
            
        std::transform(inBegin, inEnd, outBegin, [&](const auto& x){ return x / integral; });
    }
    
    //! Normalize a signal
    template <class InputIterator, class OutputIterator>
    void normalize(InputIterator inBegin, InputIterator inEnd, OutputIterator outBegin)
    {
        auto peak = *math::findAbsolutePeak(inBegin, inEnd);
        std::transform(inBegin, inEnd, outBegin, [&](const auto& x){ return x / peak; });
    }
}

#endif /* GRIZZLY_NORMALIZE_HPP */
