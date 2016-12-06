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
