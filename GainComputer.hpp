//
//  Transfer.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 30/06/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_DSP_TRANSFER_HPP
#define BEAR_DSP_TRANSFER_HPP

#include <unit/amplitude.hpp>

namespace bear::dsp
{
    //! Compressor gain estimation based on a typica mixing situation
    template <typename T1, typename T2>
    static inline unit::decibel<T1> compressorGainEstimation(const unit::decibel<T1> threshold, const T2 ratio)
    {
        return -0.5 * threshold * (1.0 / ratio - 1.0);
    }
    
    //! Downward compression for signals exceeding a threshold. Upward expansion is possible with a ratio lower than 1 (> 0)
    template <typename T>
    static inline unit::decibel<T> compressorGainComputer(const unit::decibel<T>& x, const unit::decibel<T> threshold, const T ratio, const unit::decibel<T> knee = 0.0)
    {
        auto slope = 1.0 / ratio - 1.0;
        auto halfKnee = knee * 0.5;
        
        //! Apply compression
        if (x > threshold + halfKnee)
            return slope * (x - threshold);
        
        //! Thruput factor
        if (x <= threshold - halfKnee)
            return 0.0;
        
        //! Apply compression in knee range
        else
            return slope * (x - threshold + halfKnee) * (x - threshold + halfKnee) / (2.0 * knee);
    }
    
    //! Downward expansion for signals below a threshold. Upward expansion is possible with a ratio lower than 1 (> 0)
    template <typename T>
    static inline unit::decibel<T> expanderGainFactor(const unit::decibel<T>& x, const unit::decibel<T> threshold, const T ratio, const unit::decibel<T> knee = 0.0)
    {
        auto slope = 1.0 / ratio - 1.0;
        auto halfKnee = knee * 0.5;
        
        //! Thruput factor
        if (x >= threshold + halfKnee)
            return 0.0;
        
        //! Apply expansion
        if (x < threshold - halfKnee)
            return slope * (threshold - x);
        
        //! Apply compression in knee range
        else
            return slope * (threshold + halfKnee - x) * (threshold + halfKnee - x) / (2.0 * knee);
    }
    
    

}

#endif
