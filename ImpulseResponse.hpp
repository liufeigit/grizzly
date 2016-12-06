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

#ifndef GRIZZLY_IMPULSE_RESPONSE_HPP
#define GRIZZLY_IMPULSE_RESPONSE_HPP

#include <cstdint>

namespace dsp
{
    // Create an impulse response given a filter and a size
    template <typename Iterator, typename Function>
    void createImpulseResponse(Iterator begin, Iterator end, Function&& filter)
    {
        if (begin == end)
            return;
        
        *begin = filter(1);
        for (auto it = begin + 1; it != end; ++it)
            *it = filter(0);
    }
}

#endif /* GRIZZLY_IMPULSE_RESPONSE_HPP */
