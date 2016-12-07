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

#ifndef GRIZZLY_DYNAMIC_HPP
#define GRIZZLY_DYNAMIC_HPP

#include <stdexcept>
#include <unit/amplitude.hpp>

namespace dsp
{
    //! Compressor make-up gain estimation for a 'standard' mixing situation
    inline static unit::decibel<float> compressorMakeUpGain(unit::decibel<float> threshold, float ratio)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        return 0.5f * threshold.value * (1.0f / ratio - 1.0f);
    }
    
    //! Downward compression for signals exceeding a threshold
    /*! Upward expansion is possible with a ratio lower than 1 (> 0) */
    inline static unit::decibel<float> compressDownFactor(unit::decibel<float> x, unit::decibel<float> threshold, float ratio, unit::decibel<float> knee = 0.f)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        const auto slope = 1.0f / ratio - 1.0f;
        const auto halfKnee = knee.value * 0.5f;
        
        //! Apply compression
        if (x.value > threshold.value + halfKnee)
            return slope * (x.value - threshold.value);
        
        //! Throughput
        if (x.value <= threshold.value - halfKnee)
            return 0;
        
        //! Apply compression within knee range
        return slope * (x.value - threshold.value + halfKnee) * (x.value - threshold.value + halfKnee) / (2.0f * knee.value);
    }
    
    //! Downward expansion for signals below a threshold
    /*! Upward expansion is possible with a ratio lower than 1 (> 0) */
    inline static unit::decibel<float> expandDownFactor(unit::decibel<float> x, unit::decibel<float> threshold, float ratio, unit::decibel<float> knee = 0.f)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        const auto slope = 1.0f / ratio - 1.0f;
        const auto halfKnee = knee.value * 0.5f;
        
        //! Throughput
        if (x.value >= threshold.value + halfKnee)
            return 0;
        
        //! Apply expansion
        if (x.value < threshold.value - halfKnee)
            return slope * (threshold.value - x.value);
        
        //! Apply compression within knee range
        return slope * (threshold.value + halfKnee - x.value) * (threshold.value + halfKnee - x.value) / (2.0f * knee.value);
    }
}

#endif
