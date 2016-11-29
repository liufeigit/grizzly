//
//  AnalogEnvelope.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 09/10/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef GRIZZLY_ANALOG_ENVELOPE_HPP
#define GRIZZLY_ANALOG_ENVELOPE_HPP

#include <cmath>
#include <unit/hertz.hpp>
#include <unit/proportion.hpp>
#include <unit/time.hpp>
#include <functional>

#include "FirstOrderFilter.hpp"
#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! Analog style ADSR envelope generator
    /*! Envelope generator based on a charging and discharging a capacitor.
     The curve determines the skew factor (slope) of the attack (see setAttackSkew for more info).
     By tweaking the ADSR parameters, a AR, ASR or ADSD envelope is possible. */
    template <class T, class CoeffType = double>
    class AnalogEnvelope
    {
    public:
        //! The states in which the envelope can be at any given moment
        enum class State
        {
            IDLE,
            ATTACK,
            DECAY,
            RELEASE
        };
        
    public:
        //! Construct the envelope
        AnalogEnvelope(unit::second<double> attackTime, unit::second<double> decayTime, T sustain, unit::second<double> releaseTime, unit::hertz<double> sampleRate, double attackShape = 0.77)
        {
            // Initialise attack time constant factor
            setAttackShape(attackShape);
            
            setAttackTime(attackTime, sampleRate);
            setDecayTime(decayTime, sampleRate);
            setSustain(sustain);
            setReleaseTime(releaseTime, sampleRate);
        }
        
        //! Update filter coefficients
        void updateFilter()
        {
            switch (state)
            {
                case State::IDLE: break;
                case State::ATTACK: lowPassFilter.coefficients = attackCoefficients; break;
                case State::DECAY: lowPassFilter.coefficients = decayCoefficients; break;
                case State::RELEASE: lowPassFilter.coefficients = releaseCoefficients; break;
            }
        }
        
        //! Set the attack time
        void setAttackTime(unit::second<double> time, unit::hertz<double> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, time, attackTimeConstantFactor);
            
            if (state == State::ATTACK)
                updateFilter();
        }
        
        //! Set the decay time
        void setDecayTime(unit::second<double> time, unit::hertz<double> sampleRate)
        {
            lowPassOnePole(decayCoefficients, sampleRate, time, decayReleaseTimeConstantFactor);
            
            if (state == State::DECAY)
                updateFilter();
        }
        
        
        //! Set the release time
        void setReleaseTime(unit::second<double> time, unit::hertz<double> sampleRate)
        {
            lowPassOnePole(releaseCoefficients, sampleRate, time, decayReleaseTimeConstantFactor);
            
            if (state == State::RELEASE)
                updateFilter();
        }
        
        //! Set the sustain level
        void setSustain(const double& sustain)
        {
            // multiply with maximum charge to correct for normalizing
            this->sustain = math::clamp(sustain, 0.0, 1.0) * maximumCharge;
        }
        
        //! Start the envelop by setting the mode to attack
        void start()
        {
            state = State::ATTACK;
            updateFilter();
        }
        
        //! End the envelope by setting the mode to release
        void release()
        {
            if (state != State::IDLE)
                state = State::RELEASE;
            
            updateFilter();
        }
        
        //! Sets the envelope to 0 and goes to idle state
        void reset()
        {
            state = State::IDLE;
            lowPassFilter.clear();
        }
        
        //! Returns the output of the envelope
        /*! A call to start() sets the envelope to the attack stage
            After the release() call, the envelope will shut down when the output is significantly low and returns 0. */
        T process()
        {
            switch (state)
            {
                case State::IDLE:
                    return 0;
                    
                case State::ATTACK:
                {
                    T output = lowPassFilter(gateOn);
                    if (output >= maximumCharge)
                    {
                        state = State::DECAY;
                        updateFilter();
                    }
                    
                    return output * normalizeFactor;
                }
                    
                case State::DECAY:
                    return lowPassFilter(sustain) * normalizeFactor;
                    
                case State::RELEASE:
                {
                    T output = lowPassFilter(gateOff) * normalizeFactor;
                    if (output < 0.00001) // ~ -100 dB
                    {
                        reset();
                        if (end) end();
                        return 0;
                    }
                    return output;
                }
                    
                default:
                    return 0;
            }
        }
        
        //! Returns the output of the envelope
        /*! A call to start() sets the envelope to the attack stage
            After the release() call, the envelope will shut down when the output is significantly low and returns 0. */
        T operator()() { return process(); }
        
        // Return the current state
        State getState() const { return state; }
        
    public:
        //! Called when the envelope reaches zero. Useful for updating some code elsewhere (e.g. free a voice)
        std::function<void()> end = [](){};
        
    private:
        //! Set the shape, from flat to steap, of the attack
        /*! The shape of the attack is determined by the maximum charge of a 'capacitor' (0.1 - 0.99)
         By default, the maximu charge is 77% and approximates a CEM3310 chip.
         The output (y) of charging is given by: y(n) = 1 - e^(-n/RC), where time constant RC is set to 1 and time n is in samples.
         At n = 1, y = 0.63. We need 5 samples to a full charge of 0.99. If we replace n with -5, we can go from 0 to 0.99 in one tick (1 - e^-5)
         and use this value to set a maximum charge. In this case 5 is the time constant factor to make this step in one sample.
         We can then solve time constant factors for different charges between 0% and 100% with maximum charge = 1 - e^-factor.
         If we then normalize the envelope, the maximum charge results in changing the attack shape. */
        void setAttackShape(unit::proportion<double> maximumCharge)
        {
            maximumCharge = math::clamp<double>(maximumCharge, 0.1, 0.99);
            this->maximumCharge = maximumCharge;
            normalizeFactor = 1.0 / maximumCharge;
            
            // solve time constant factor for maximum charge = 1 - e^-factor
            long double temp = 1.l - maximumCharge;
            attackTimeConstantFactor = -log(temp);
        }
        
    private:
        //! The low-pass filter output is the envelope
        FirstOrderFilter<T, CoeffType> lowPassFilter;
        
        //! The attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! The decay coefficients
        FirstOrderCoefficients<CoeffType> decayCoefficients;
        
        //! The release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! The Envelope State
        State state = State::IDLE;
        
        //! Maximum charge for the capacitor
        unit::proportion<double> maximumCharge = 0.77;
        
        //! The normalise factor
        double normalizeFactor = 1.0 / maximumCharge;
        
        //! The sustain level
        /*! The sustain is multiplied with the maximum charge to correct for nomalizing */
        T sustain = 0.0;
        
        //! Time constant factor for the attack
        double attackTimeConstantFactor = 1;
        
        //! Time constant factor for decay and release. A factor of 4.95 aproximates a real-world capacitor
        const double decayReleaseTimeConstantFactor = 4.95;
        
        //! A gate signal of 1
        const double gateOn = 1.0;
        
        //! A gate signal of 0
        const double gateOff = 0.0;
    };
}

#endif
