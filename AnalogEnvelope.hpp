//
//  AnalogEnvelope.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 09/10/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef BEAR_DSP_ANALOG_ENVELOPE_HPP
#define BEAR_DSP_ANALOG_ENVELOPE_HPP

#include <cmath>
#include <unit/hertz.hpp>
#include <unit/proportion.hpp>
#include <unit/time.hpp>

#include "FirstOrderFilter.hpp"
#include "Math.hpp"

namespace bear::dsp
{
    //! Analog style ADSR envelope generator
    /*! Envelope generator based on a charging and discharging a capacitor.
        The curve determines the skew factor (slope) of the attack (see setAttackSkew for more info).
        By tweaking the ADSR parameters, a AR, ASR or ADSD envelope is possible. */
    template <class T, class CoeffType = double>
    class AnalogEnvelope
    {
    public:
        //! The states in which the envelope functions
        enum class State
        {
            IDLE,
            ATTACK,
            DECAY,
            RELEASE
        };
        
    public:
        AnalogEnvelope()
        {
            // Initialise attack time constant factor
            setAttackShape(0.77);
        }
        
        void setSampleRate(unit::hertz<float> sampleRate)
        {
            this->sampleRate = sampleRate;
        }
        
        //! Sets the shape, flat to steap, of the attack
        /*! The shape of the attack is determined by the curve factor (clamped to > 0 & < 1).
            A higher value results in a steaper curve and is analogous to the maximum charge of a capacitor.
            By default, the setting is 0.77 and approximates a CEM3310 chip wichh charges to a maximum of 77 percent.
         
            The output (y) of charging is given by: y(n) = 1 - e^(-n/RC), where time constant RC is set to 1 and time n is in samples.
            At n = 1, y = 0.63. We need 5 samples to a full charge of 0.99. In other words, we can go from 0 to 0.99 in one sample (1 - e^((-1 * 5) / 1))
            and use this value to set a maximum charge. In this case 5 is the time constant factor to make this step in one sample.
         
            We can then solve time constant factors for different charges between 0% and 100% with maximum charge = 1 - e^-factor. If we normalise the envelope, the factor determines the shape.
            The envelope is the output of a one-pole filter using a gate signal of 1 or 0 as input. */
        void setAttackShape(unit::proportion<float> maximumCharge)
        {
            maximumCharge = clamp<float>(maximumCharge, 0.1, 0.99);
            this->maximumCharge = maximumCharge;
            normaliseFactor = 1.0 / maximumCharge;
            
            // solve time constant factor for maximum charge = 1 - e^-factor
            double temp = 1.0 - maximumCharge;
            attackTimeConstantFactor = -log(temp);
        }
        
        void setAttackTime(unit::millisecond<float> time)
        {
            if (time <= 0)
                time = 1;
            
            attackTime = time;
        }
        
        void setDecayTime(unit::millisecond<float> time)
        {
            if (time <= 0)
                time = 1;
            
            decayTime = time;
        }
        
        void setReleaseTime(unit::millisecond<float> time)
        {
            if (time <= 0)
                time = 1;
            
            releaseTime = time;
        }
        
        void setSustain(T sustain)
        {
            this->sustain = sustain;
        }
        
        //! Start the envelop by setting the mode to attack
        void start()
        {
            state = State::ATTACK;
            
            // set coefficients
            lowPassOnePole(lowPassFilter.coefficients, sampleRate, attackTime, attackTimeConstantFactor);
        }
        
        //! End the envelope by setting the mode to release
        void release()
        {
            if (state != State::IDLE)
                state = State::RELEASE;
            
            // set coefficients
            lowPassOnePole(lowPassFilter.coefficients, sampleRate, releaseTime, decayReleaseTimeConstantFactor);
        }
        
        //! Sets the envelope to 0 and goes to idle state
        void reset()
        {
            lowPassFilter.clear();
            state = State::IDLE;
        }
        
        //! Returns the output of the envelope
        /*! A call to start() sets the envelope to the attack stage
            After the release() call, the envelope will shut down when the output is significantly low and returns 0. */
        T operator()()
        {
            T output = 0;
            
            switch (state)
            {
                case State::IDLE:
                    return output;
                    
                case State::ATTACK:
                {
                    output = lowPassFilter(gateOn);
                    
                    if (output >= maximumCharge)
                    {
                        state = State::DECAY;
                        
                        // set coefficients
                        lowPassOnePole(lowPassFilter.coefficients, sampleRate, decayTime, decayReleaseTimeConstantFactor);
                    }
                    
                    break;
                }
                    
                case State::DECAY:
                {
                    // sustain is adjusted with the maximum charge because of the normalisation factor before return
                    auto adjustedSustain = sustain * maximumCharge;
                    
                    output = lowPassFilter(adjustedSustain);
        
                    break;
                }
                    
                case State::RELEASE:
                {
                    output = lowPassFilter(gateOff);
                    
                    if (output < 0.00001) // ~ -100 dB
                    {
                        state = State::IDLE;
                        lowPassFilter.clear();
                        if (done)
                            done();
                        return 0;
                    }

                    break;
                }
                    
                // Should never get here
                default:
                    return 0;
            }
            
            // Multiply with the normalise factor to get a ouput range between 0 and 1
            return output * normaliseFactor;
        }
        
        //! Called when the envelope reaches zero. Useful for updating some code elsewhere (e.g. free a voice)
        std::function<void()> done;

        unit::second<float> getAttackTime() const { return attackTime; }
        unit::second<float> getDecayTime() const { return decayTime; }
        unit::second<float> getReleaseTime() const { return releaseTime; }
        
        State getState() const { return state; }
        
    private:
        //! The low-pass filter output is the envelope
        FirstOrderFilter<T, CoeffType> lowPassFilter;
        
        //! The Envelope State
        State state = State::IDLE;
        
        //! The sample rate
        unit::hertz<float> sampleRate = 44100;
        
        //! Maximum charge for the capacitor
        unit::proportion<float> maximumCharge = 0.77; // CEM chip specs
        
        //! The normalise factor makes the output in the range 0 to 1
        double normaliseFactor = 1.0 / maximumCharge;
        
        //! Time constant factor for the attack, set in setAttackShape()
        double attackTimeConstantFactor = 1;
        
        //! Time constant factor for decay and release. A factor of 4.95 aproximates a real-world capacitor
        double decayReleaseTimeConstantFactor = 4.95;
        
        //! The attack time
        unit::second<float> attackTime = 0.1;
        
        //! The decay time
        unit::second<float> decayTime = 0.1;
        
        //! The rlease time
        unit::second<float> releaseTime = 0.1;
        
        //! The sustain level (range of 0 to 1)
        T sustain = 0.5;
        
        //! A gate signal of 1
        const double gateOn = 1.0;
        
        //! A gate signal of 0
        const double gateOff = 0.0;
    };
}

#endif
