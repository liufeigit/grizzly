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
        //! Default constructor
        AnalogEnvelope(unit::hertz<double> sampleRate) :
            sampleRate(sampleRate)
        {
            // Initialize the constant factors per stage
            setAttackShape(0.77);
            decayStage.timeConstantReciprocal = 4.95;
            releaseStage.timeConstantReciprocal = 4.95;
            
            setAttackTime(0.1);
            setDecayTime(0.1);
            setSustain(0.5);
            setReleaseTime(0.1);
        }
        
        //! Construct the envelope
        AnalogEnvelope(unit::hertz<double> sampleRate, unit::second<double> attackTime, unit::second<double> decayTime, T sustain, unit::second<double> releaseTime, double attackShape = 0.77) :
            sampleRate(sampleRate)
        {
            // Initialize the constant factors per stage
            setAttackShape(attackShape);
            decayStage.timeConstantReciprocal = 4.95;
            releaseStage.timeConstantReciprocal = 4.95;
            
            setAttackTime(attackTime);
            setDecayTime(decayTime);
            setSustain(sustain);
            setReleaseTime(releaseTime);
        }
        
        //! Set the attack time
        void setAttackTime(unit::second<double> time)
        {
            attackStage.set(time, sampleRate);
            
            if (state == State::ATTACK)
                updateFilterCoefficients();
        }
        
        //! Set the decay time
        void setDecayTime(unit::second<double> time)
        {
            decayStage.set(time, sampleRate);
            
            if (state == State::DECAY)
                updateFilterCoefficients();
        }
        
        
        //! Set the release time
        void setReleaseTime(unit::second<double> time)
        {
            releaseStage.set(time, sampleRate);
            
            if (state == State::RELEASE)
                updateFilterCoefficients();
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
            updateFilterCoefficients();
        }
        
        //! End the envelope by setting the mode to release
        void release()
        {
            if (state != State::IDLE)
                state = State::RELEASE;
            
            updateFilterCoefficients();
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
                        updateFilterCoefficients();
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
        /*! see Stage::timeConstantReciprocal for more information */
        void setAttackShape(unit::proportion<double> maximumCharge)
        {
            maximumCharge = math::clamp<double>(maximumCharge, 0.1, 0.99);
            this->maximumCharge = maximumCharge;
            normalizeFactor = 1.0 / maximumCharge;
            
            // solve time constant factor for maximum charge = 1 - e^-timeConstantReciprocal
            long double temp = 1.l - maximumCharge;
            attackStage.timeConstantReciprocal = -log(temp);
        }
        
        //! Make sure we're using the correct coefficients
        void updateFilterCoefficients()
        {
            switch (state)
            {
                case State::IDLE: break;
                case State::ATTACK: lowPassFilter.coefficients = attackStage.coefficients; break;
                case State::DECAY: lowPassFilter.coefficients = decayStage.coefficients; break;
                case State::RELEASE: lowPassFilter.coefficients = releaseStage.coefficients; break;
            }
        }
        
    private:
        //! The envelope consists goes through three stages, each needing the same set of variables
        class Stage
        {
        public:
            void set(unit::second<double> time, unit::hertz<double> sampleRate)
            {
                lowPassOnePole(coefficients, sampleRate, time, timeConstantReciprocal);
            }
            
        public:
            //! The filter coefficients set to function as the state
            FirstOrderCoefficients<CoeffType> coefficients;
            
            //! The shape of the filter curve is determined by the maximum charge of a 'capacitor' (0.1 - 0.99)
            /*! By default, the maximum charge is 77% and approximates a CEM3310 chip.
                The output (y) of charging is given by: y(n) = 1 - e^(-n/RC), where time constant RC is set to 1 and time n is in samples.
                At n = 1, y = 0.63. We need 5 samples to a full charge of 0.99. If we replace n with -5, we can go from 0 to 0.99 in one tick (1 - e^-5)
                and use this value to set a maximum charge. In this case 5 is the time constant factor to make this step in one sample.
                We can then solve time constant factors for different charges between 0% and 100% with maximum charge = 1 - e^-factor.
                If we then normalize the envelope, the maximum charge results in changing the filter shape. */
            double timeConstantReciprocal = 0;
        };
        
    private:
        //! The output of the envelope is generated using a simple low-pass filter
        FirstOrderFilter<T, CoeffType> lowPassFilter;
        
        //! The envelope consists of three different stages, each with their own coefficients
        Stage attackStage;
        Stage decayStage;
        Stage releaseStage;
        
        //! The Envelope State
        State state = State::IDLE;
        
        //! The sample rate at which the envelope runs
        unit::hertz<double> sampleRate;
        
        //! Maximum charge for the capacitor
        unit::proportion<double> maximumCharge = 0.77;
        
        //! The normalise factor
        double normalizeFactor = 1.0 / maximumCharge;
        
        //! The sustain level
        /*! The sustain is multiplied with the maximum charge to correct for nomalizing */
        T sustain = 0.0;
        
        //! A gate signal of 1
        const double gateOn = 1.0;
        
        //! A gate signal of 0
        const double gateOff = 0.0;
    };
}

#endif
