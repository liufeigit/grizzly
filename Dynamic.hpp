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

#ifndef GRIZZLY_WAVESHAPER_HPP
#define GRIZZLY_WAVESHAPER_HPP

#include <stdexcept>
#include <unit/amplitude.hpp>

namespace dsp
{
    //! Compressor make-up gain estimation for a 'standard' mixing situation
    inline static unit::decibel<float> compressorMakeUpGain(const unit::decibel<float>& threshold, float ratio)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        return 0.5 * threshold * (1.0 / ratio - 1.0);
    }
    
    //! Downward compression for signals exceeding a threshold
    /*! Upward expansion is possible with a ratio lower than 1 (> 0) */
    inline static unit::decibel<float> compressDownFactor(const unit::decibel<float>& x, const unit::decibel<float>& threshold, float ratio, const unit::decibel<float>& knee = 0.f)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        const auto slope = 1.0 / ratio - 1.0;
        const auto halfKnee = knee * 0.5;
        
        //! Apply compression
        if (x > threshold + halfKnee)
            return slope * (x - threshold);
        
        //! Throughput
        if (x <= threshold - halfKnee)
            return 0;
        
        //! Apply compression within knee range
        return slope * (x - threshold + halfKnee) * (x - threshold + halfKnee) / (2.0 * knee);
    }
    
    //! Downward expansion for signals below a threshold
    /*! Upward expansion is possible with a ratio lower than 1 (> 0) */
    inline static unit::decibel<float> expandDownFactor(const unit::decibel<float>& x, const unit::decibel<float>& threshold, float ratio, const unit::decibel<float>& knee = 0.f)
    {
        if (ratio <= 0)
            throw std::invalid_argument("ratio <= zero");
        
        const auto slope = 1.0 / ratio - 1.0;
        const auto halfKnee = knee * 0.5;
        
        //! Throughput
        if (x >= threshold + halfKnee)
            return 0;
        
        //! Apply expansion
        if (x < threshold - halfKnee)
            return slope * (threshold - x);
        
        //! Apply compression within knee range
        return slope * (threshold + halfKnee - x) * (threshold + halfKnee - x) / (2.0 * knee);
    }
}

#endif
