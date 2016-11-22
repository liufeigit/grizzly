//
//  Ramp.hpp
//  bear
//
//  Created by Milan van der Meer on 25/07/16.
//
//

#ifndef GRIZZLY_RAMP_HPP
#define GRIZZLY_RAMP_HPP

#include <functional>
#include <stdexcept>
#include <unit/hertz.hpp>

#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! A linearly as/descending ramp signal
    /*! Outputs the current state and increments (or decrements if negative) with each call.
        Output is then wrapped within a normalized range and can be used as an oscillator,
        or more typical, to drive the phase of another oscillator. */
    template <class T>
    class Ramp
    {
    public:
        //! Default constructor
        Ramp() = default;
        
        //! Construct the ramp with an initial phase
        Ramp(const T& phase)
        {
            setPhase(phase);
        }
        
        //! Set the phase for next callback, wrap if necessary
        void setPhase(const T& phase)
        {
            this->phase = math::wrap<T>(phase, 0, 1);
        }
        
        //! Increment the phase
        void increment(const T& increment)
        {
            phase += increment;
            if (phase >= 1 && end)
                end();
            
            phase = math::wrap<T>(phase, 0, 1);
        }
        
        //! Get phase
        T getPhase() const
        {
            return phase;
        }
        
    public:
        //! End function when ramp gets wrapped
        std::function<void(void)> end;

    private:
        //! Phase
        T phase = 0;
    };
}

#endif
