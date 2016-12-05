//
//  HighFrequencyContent.hpp
//  libbear
//
//  Created by Stijn Frishert on 11/12/15.
//  Copyright © 2015 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_HIGH_FREQUENCY_CONTENT_HPP
#define GRIZZLY_HIGH_FREQUENCY_CONTENT_HPP

#include <unit/hertz.hpp>

namespace dsp
{
    //! Retrieve the high-frequency content according to Brossier
    template <typename Iterator>
    constexpr double highFrequencyContentBrossier(Iterator begin, Iterator end)
    {
        typename Iterator::value_type acc = 0;
        const auto size = std::distance(begin, end);
        for (auto i = 0; i < size; ++i)
            acc += *begin++ * i;
        
        return acc / static_cast<double>(size);
    }
    
    //! Retrieve the high-frequency content according to Masri
    template <typename Iterator>
    constexpr double highFrequencyContentMasri(Iterator begin, Iterator end)
    {
        typename Iterator::value_type acc = 0;
        const auto size = std::distance(begin, end);
        for (auto i = 0; i < size; ++i)
        {
            acc += i * (*begin * *begin);
            ++begin;
        }
        
        return acc / static_cast<double>(size);
    }
    
    //! Retrieve the high-frequency content according to Jensen
    template <typename Iterator>
    constexpr double highFrequencyContentJensen(Iterator begin, Iterator end)
    {
        typename Iterator::value_type acc = 0;
        const auto size = std::distance(begin, end);
        for (auto i = 0; i < size; ++i)
            acc += *begin++ * (i * i);
        
        return acc / static_cast<double>(size);
    }
}

#endif
