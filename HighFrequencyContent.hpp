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
