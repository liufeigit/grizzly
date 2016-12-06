//
//  Transfer.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

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
