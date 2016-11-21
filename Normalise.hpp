//
//  Normalise.hpp
//  grizzly
//
//  Created by Milan van der Meer on 16/11/16.
//
//

#ifndef Normalise_hpp
#define Normalise_hpp

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <dsperados/math/analysis.hpp>
#include <numeric>
#include <stdexcept>
#include <vector>

namespace dsp
{
    //! Normalise an area so the integral of the signal equals one
    template <class InputIterator, class OutputIterator>
    static inline void normaliseArea(InputIterator inBegin, InputIterator inEnd, OutputIterator outBegin)
    {
        auto integral = std::accumulate(inBegin, inEnd, 0.l);
        
        if (integral)
            std::transform(inBegin, inEnd, outBegin, [&](const auto& x){ return x / integral; });
    }
    
    //! Normalise a signal
    template <class InputIterator, class OutputIterator>
    static inline void normalise(InputIterator inBegin, InputIterator inEnd, OutputIterator outBegin)
    {
        std::common_type_t<decltype(*inBegin), decltype(*outBegin)> peak = math::absolutePeak(std::vector<decltype(*inBegin)>(inBegin, inEnd));
        std::transform(inBegin, inEnd, outBegin, [&](const auto& x){ return x / peak; });
    }
}

#endif /* Normalise_hpp */
