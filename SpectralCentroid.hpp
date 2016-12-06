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

#ifndef GRIZZLY_SPECTRAL_CENTROID_HPP
#define GRIZZLY_SPECTRAL_CENTROID_HPP

#include <unit/hertz.hpp>

namespace dsp
{
    //! The centroid of a range of values
    /*! The centroid or 'center of gravity' is sum of values, weighted by its index, divided by the sum of values. */
    template <typename Iterator>
    constexpr double spectralCentroid(Iterator begin, Iterator end)
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
        
        return numerator / static_cast<double>(denominator);
    }
}

#endif /* GRIZZLY_SPECTRAL_CENTROID_HPP */
