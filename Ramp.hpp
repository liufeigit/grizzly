//
//  Ramp.hpp
//  bear
//
//  Created by Milan van der Meer on 25/07/16.
//
//

#ifndef BEAR_DSP_RAMP_HPP
#define BEAR_DSP_RAMP_HPP

#include <functional>
#include <stdexcept>
#include <unit/hertz.hpp>

#include <dsperados/math/utility.hpp>

namespace bear::dsp
{
    //! A linearly as/descending ramp signal
    /*! Outputs the current state and increments (or decrements if negative) with each call.
        Output is then wrapped within its range and can therefore be used as an oscillator,
        or more typical, to drive the phase of another oscillator. */
    template <class T>
    class Ramp
    {
    public:
        //! Default constructor
        /*! Range goes from 0 to 1 */
        Ramp() = default;
        
        //! Construct the ramp
        Ramp(const T& min, const T& max) :
            min(min),
            max(max)
        {
            if (max < min)
                throw std::invalid_argument("negative range not allowed");
            
            setState(range.offset);
        }
        
        //! Set the ramp value for next callback
        void setState(const T& value)
        {
            if (value >= range.max())
                end();
            
            y = math::wrap(value, min, max);
        }
        
        //! Output the current ramp value, increment and wrap (if necessary) for next callback
        T operator()(const T& increment)
        {
            T output = y;
            setState(y + increment);
            
            return output;
        }
        
    public:
        std::function<void(void)> end;

    private:
        //! The minimal value of the range
        T min = 0;
        
        //! The maximal value of the range
        T max = 1;
        
        //! Ramp output value
        T y = 0;
    };
}

#endif
