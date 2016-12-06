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

#ifndef GRIZZLY_ALL_PASS_FILTER_HPP
#define GRIZZLY_ALL_PASS_FILTER_HPP

#include <dsperados/math/constants.hpp>
#include <vector>

#include "Delay.hpp"

using namespace std;

namespace dsp
{
    template <class T>
    class AllPassFilter
    {
    public:
        AllPassFilter(std::size_t maxDelayTime) :
            delay(maxDelayTime)
        {

        }

        T process(const T& x, float delayTime, float gain = math::INVERSE_PHI<float>)
        {
            const auto w = gain * delay.read(delayTime) + x;
            const auto y = gain * w - delay.read(delayTime);

            delay.write(w);

            return y;
        }
        
        T operator()(const T& x, float delayTime, float gain = math::INVERSE_PHI<float>)
        {
            return process(x, delayTime, gain);
        }

        T getMaximumDelayTime()
        {
            return delay.getMaximumDelayTime();
        }

    private:
        Delay<T> delay;
    };
}


#endif
