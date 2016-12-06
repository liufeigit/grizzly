//
//  EnvelopeDetector.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 11/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef GRIZZLY_ENVELOPE_DETECTOR_HPP
#define GRIZZLY_ENVELOPE_DETECTOR_HPP

#include <cmath>
#include <dsperados/math/utility.hpp>

#include "FirstOrderCoefficients.hpp"
#include "FirstOrderFilter.hpp"

namespace dsp
{
    //! Envelope detector based on an analog circuit with two resistors, a capacitor and a diode
    /*! The capacitor continuously discharges via the release resistor. Due to this design, the peak will not
        reach its maximum value. choosing a bigger release time makes this less noticeably. See "Investigation
        in Dynamic Range Compression" by Massberg. */
    template <class T, class CoeffType = double>
    class EnvelopeDetectorRCR
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp */
        EnvelopeDetectorRCR(double timeConstantFactor = 5) :
            timeConstantFactor(timeConstantFactor)
        {
            
        }
        
        //! Write new input to the detector
        void write(const T& x)
        {
            y = releaseCoefficients.b1 * y + attackCoefficients.a0 * std::max<T>(x - y, 0);
        }
        
        //! Read the last computed value
        T read() const { return y; }
        
        //! Set the attack time
        void setAttackTime(unit::second<float> attackTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
        }
        
        //! Set the attack time
        void setReleaseTime(unit::second<float> releaseTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
        //! Set envelope state
        void setState(const T& y) { this->y = y; }
        
    private:
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        double timeConstantFactor = 5;
        
        //! The most recently computed value
        T y = 0;
    };

    //! Envelope detector based on an analog circuit with two resistor, two capacitors and a diode
    /*! The first set of resistor/capacitor is decoupled from the second set. In contrast to the EnvelopeDetectorRCR,
        the envelope will reach the maximum value regardless of different release settings. See "Investigation in Dynamic
        Range Compression" by Massberg. */
    template <class T, class CoeffType = double>
    class EnvelopeDetectorDecoupled
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp */
        EnvelopeDetectorDecoupled(double timeConstantFactor = 5) :
            timeConstantFactor(timeConstantFactor)
        {
            
        }
        
        //! Write new input to the detector
        T write(const T& x)
        {
            yRelease = std::max(x, yRelease - releaseCoefficients.a0 * yRelease);
            y += attackCoefficients.a0 * (yRelease - y);
        }
        
        //! Read the last computed value
        T read() const { return y; }
        
        //! Set the attack time
        void setAttackTime(unit::second<float> attackTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
        }
        
        //! Set the attack time
        void setReleaseTime(unit::second<float> releaseTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
        //! Set envelope state
        void setState(const T& y) { this->y = y; }
        
    private:
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        double timeConstantFactor = 5;
        
        //! Intermediate release state
        T yRelease = 0;
        
        //! The most recently computed value
        T y = 0;
    };
    
    //! Envelope detector using a one-pole low-pass filter
    /*! Notice the input is not rectified */
    template <class T, class CoeffType = double>
    class EnvelopeDetectorDigital
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp 
            @Param releaseToInput Setting this to true means that the release trajectory will reach to the current input. Else, the envelope will descend to zero. */
        EnvelopeDetectorDigital(bool releaseToInput, double timeConstantFactor = 5) :
            timeConstantFactor(timeConstantFactor),
            releaseToInput(releaseToInput)
        {
            
        }
        
        //! Write new input to the detector
        void write(const T& x)
        {
            if (x > y)
            {
                lowPassFilter.coefficients = attackCoefficients;
                y = lowPassFilter(x);
            } else {
                lowPassFilter.coefficients = releaseCoefficients;
                releaseToInput ? y = lowPassFilter(x) : y = lowPassFilter(0);
            }
        }
        
        //! Read the last computed value
        T read() const { return y; }
        
        //! Set the attack time
        void setAttackTime(unit::second<float> attackTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
        }
        
        //! Set the attack time
        void setReleaseTime(unit::second<float> releaseTime, unit::hertz<float> sampleRate)
        {
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
        //! Set envelope state
        void setState(const T& y) { this->y = y; }
        
    private:
        //! First order filter
        FirstOrderFilter<T, CoeffType> lowPassFilter;
        
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        double timeConstantFactor = 5;
        
        //! Boolian for release mode
        bool releaseToInput = true;
        
        //! The most recently computed value
        T y = 0;
    };
    
}

#endif /* GRIZZLY_ENVELOPEDETECTOR_HPP */
