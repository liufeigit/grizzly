//
//  Transfer.hpp
//  bear-core
//
//  Created by Milan van der Meer on 19/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_Z_TRANSFORM_HPP
#define GRIZZLY_Z_TRANSFORM_HPP

#include <complex>

#include <unit/radian.hpp>

namespace dsp
{
    //! Apply z-transform on a input sequence and return the transfer function
    /*! The transfer function can be used to retrieve the spectrum bin of your input sequence
        at a specific frequency. The magnitude and angle of the returned complex give provide
        you the amplitude and phase of the given frequency. */
    template <typename Iterator>
    auto zTransform(Iterator begin, Iterator end)
    {
        return [=](const unit::radian<float>& angularFrequency)
        {
            std::complex<float> accumulator(0, 0);
            size_t index = 0;
            
            for (auto it = begin; it != end; ++it)
                accumulator += *it * std::polar<float>(1, -angularFrequency * index++);

            return accumulator;
        };
    }
}

#endif
