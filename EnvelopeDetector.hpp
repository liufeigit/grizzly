//
//  EnvelopeDetector.hpp
//  bear-dsp
//
//  Created by Milan van der Meer on 11/07/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//

#ifndef BEAR_DSP_ENVELOPEDETECTOR_HPP
#define BEAR_DSP_ENVELOPEDETECTOR_HPP

#include "FirstOrderCoefficients.hpp"
#include "FirstOrderFilter.hpp"
#include <dsperados/math/utility.hpp>

namespace bear::dsp
{
    //! Abstract bass class for envelope detectors
    template <class T>
    class EnvelopeDetector
    {
    public:
        //! Virtual destructor
        virtual ~EnvelopeDetector() {}
        
        //! Implement this process function for detection behaviour
        virtual T operator()(const T& x) = 0;
        
        //! Set envelope state
        void setState(const T& y)
        {
            this->y = y;
        }
        
    protected:
        //! Envelope state
        T y = 0;
    };
    
    //! Envelope detector based on an analog circuit with two resistors, a capacitor and a diode (half-wave recification) with different coefficients for attack and release
    /*! The capacitor continuously discharges via the release resistor. Due to this design, the peak will not reach its maximum value. choosing a bigger release time makes this less noticeably. See "Investigation in Dynamic Range Compression" by Massberg. */
    template <class T, class CoeffType = double>
    class EnvelopeDetectorRCR : public EnvelopeDetector<T>
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp */
        EnvelopeDetectorRCR(CoeffType timeConstantFactor) : timeConstantFactor(timeConstantFactor)
        {
        }
        
        //! Return the followed envelope of the input
        T operator()(const T& x) override
        {
            this->y = releaseCoefficients.b1 * this->y + attackCoefficients.a0 * std::max<T>(x - this->y, 0);
            return this->y;
        }
        
        //! Set coefficients
        void setCoefficients(unit::second<CoeffType> attackTime, unit::second<CoeffType> releaseTime, unit::hertz<CoeffType> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
    private:
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        CoeffType timeConstantFactor = 0;
    };
    
    

    //! Envelope detector based on an analog circuit with two resistor, two capacitors and a diode (half-wave recification) with different coefficients for attack and release
    /*! The first set of resistor/capacitor is decoupled from the second set. In contrary to the EnvelopeDetectorRCR, the envelope will reach the maximum value regardless of different release settings. See "Investigation in Dynamic Range Compression" by Massberg. */
    template <class T, class CoeffType = double>
    class EnvelopeDetectorDecoupled : public EnvelopeDetector<T>
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp */
        EnvelopeDetectorDecoupled(CoeffType timeConstantFactor) : timeConstantFactor(timeConstantFactor)
        {
        }
        
        //! Return the followed envelope of the input
        T operator()(const T& x) override
        {
            yRelease = std::max(x, yRelease - releaseCoefficients.a0 * yRelease);
            this->y = this->y + attackCoefficients.a0 * (yRelease - this->y);
            return this->y;
        }
        
        //! Set coefficients
        void setCoefficients(unit::second<CoeffType> attackTime, unit::second<CoeffType> releaseTime, unit::hertz<CoeffType> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
    private:
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        CoeffType timeConstantFactor = 0;
        
        //! Intermediate release state
        T yRelease = 0;
    };
    
    //! Envelope detector using a one-pole low-pass filter with different coefficients for attack and release. Notice the input is not rectified.
    template <class T, class CoeffType = double>
    class EnvelopeDetectorDigital : public EnvelopeDetector<T>
    {
    public:
        //! Construct the detector
        /*! @param timeConstantFactor see FirstOrderCoefficients.hpp 
            @Param releaseToInput Setting this to true means that the release trajectory will reach to the current input. Else, the envelope will descend to zero. */
        EnvelopeDetectorDigital(CoeffType timeConstantFactor, bool releaseToInput) :
            timeConstantFactor(timeConstantFactor),
            releaseToInput(releaseToInput)
        {
        }
        
        //! Return the followed envelope of the input
        T operator()(const T& x)
        {
            if (x > this->y)
            {
                lowPassFilter.coefficients = attackCoefficients;
                this->y = lowPassFilter(x);
            }
            else
            {
                lowPassFilter.coefficients = releaseCoefficients;
                releaseToInput ? this->y = lowPassFilter(x) : this->y = lowPassFilter(0);
            }
            
            return this->y;
        }
        
        //! Set coefficients
        void setCoefficients(unit::second<CoeffType> attackTime, unit::second<CoeffType> releaseTime, unit::hertz<CoeffType> sampleRate)
        {
            lowPassOnePole(attackCoefficients, sampleRate, attackTime, timeConstantFactor);
            lowPassOnePole(releaseCoefficients, sampleRate, releaseTime, timeConstantFactor);
        }
        
    private:
        //! First order filter
        FirstOrderFilter<T, CoeffType> lowPassFilter;
        
        //! Attack coefficients
        FirstOrderCoefficients<CoeffType> attackCoefficients;
        
        //! Release coefficients
        FirstOrderCoefficients<CoeffType> releaseCoefficients;
        
        //! Time constant factor
        CoeffType timeConstantFactor = 0;
        
        //! Boolian for release mode
        bool releaseToInput = true;
    };
    
}

#endif /* BEAR_DSP_ENVELOPEDETECTOR_HPP */
