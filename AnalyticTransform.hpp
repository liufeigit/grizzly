//
//  AnalyticTransform.hpp
//  libbear
//
//  Created by Stijn Frishert on 03/02/16.
//  Copyright © 2016 FrisHertz. All rights reserved.
//

#ifndef GRIZZLY_ANALYTIC_TRANSFORM_HPP
#define GRIZZLY_ANALYTIC_TRANSFORM_HPP

#include <complex>
#include <vector>

#include "HilbertTransform.hpp"

namespace dsp
{
    //! The analytic transform of a real signal
    template <typename InputIterator, typename OutputIterator>
    void analyticTransform(InputIterator begin, InputIterator end, OutputIterator outBegin)
    {
        // Take the hilbert transform
        const auto size = std::distance(begin, end);
        std::vector<typename InputIterator::value_type> hilbert(size);
        hilbertTransform(begin, end, hilbert.begin(), HilbertTransformDirection::FORWARD);
        
        // Emplace the original signal and its hilbert transform in a complex signal
        for (auto& x : hilbert)
        {
            outBegin->real(*begin++);
            outBegin->imag(x);
            ++outBegin;
        }
    }
}

#endif /* GRIZZLY_ANALYTIC_TRANSFORM_HPP */
