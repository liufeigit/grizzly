/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
