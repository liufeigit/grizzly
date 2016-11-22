//
//  AnalogLadderFilter.hpp
//  Sandbox Milan
//
//  Created by Milan van der Meer on 26/10/15.
//  Copyright © 2015 Milan van de Meer. All rights reserved.
//

#ifndef GRIZZLY_ANALOG_LADDER_FILTER_HPP
#define GRIZZLY_ANALOG_LADDER_FILTER_HPP

#include <cmath>
#include <experimental/optional>
#include <unit/hertz.hpp>

#include "AnalogOnePoleFilter.hpp"
#include <dsperados/math/utility.hpp>

namespace dsp
{
    //! Topology preserving ladder filter with resolved zero feedback delay
    /*! See "Designing software synthesizer plug-ins in c++" by Will Pirkle.
        See "The Art Of VA Filter Design" by Vadim Zavalishin. */
    template <class T1, class T2 = double>
    class AnalogLadderFilter
    {
    public:
        //! Oberheim Xpander modes
        enum class XpanderMode
        {
            SECONDORDERLOWPASS,
            SECONDORDERHIGHPASS,
            SECONDORDERBANDPASS,
            FOURTHORDERBANDPASS,
            FOURTHORDERHIGHPASS
        };

    public:
        //! Set coefficients
        void setCoefficients(unit::hertz<T2> cutOff, const T2& feedbackFactor, unit::hertz<T2> sampleRate)
        {
            this->feedbackFactor = feedbackFactor;
            auto integratorGainFactor = std::tan(PI<T2> * cutOff / sampleRate);
            auto gainFactorOnePole = integratorGainFactor / (1.0 + integratorGainFactor);
            
            stage1.setGainFactor(gainFactorOnePole);
            stage2.setGainFactor(gainFactorOnePole);
            stage3.setGainFactor(gainFactorOnePole);
            stage4.setGainFactor(gainFactorOnePole);
            
            feedbackFactorLP1 = gainFactorOnePole * gainFactorOnePole * gainFactorOnePole / (1.0 + integratorGainFactor);
            feedbackFactorLP2 = gainFactorOnePole * gainFactorOnePole / (1.0 + integratorGainFactor);
            feedbackFactorLP3 = gainFactorOnePole / (1.0 + integratorGainFactor);
            feedbackFactorLP4 = 1.0 / (1.0 + integratorGainFactor);
            
            gainFactor = 1.0 / (1.0 + feedbackFactor * (gainFactorOnePole * gainFactorOnePole * gainFactorOnePole * gainFactorOnePole));
        }
        
        //! Set optional distortion factor for non-linear processing
        void setDistortion(std::experimental::optional<T2> factor)
        {
            distortionFactor = factor;
            stage1.setDistortion(factor);
            stage2.setDistortion(factor);
            stage3.setDistortion(factor);
            stage4.setDistortion(factor);
        }
        
        //! Set pass-band gain compensation
        void setPassBandGain(bool gain)
        {
            passBandGain = gain;
        }
        
        //! Process and return the output with optional distortion
        T1 operator()(const T1& x)
        {
            auto feedbackSum = feedbackFactorLP1 * stage1.getState() +
                               feedbackFactorLP2 * stage2.getState() +
                               feedbackFactorLP3 * stage3.getState() +
                               feedbackFactorLP4 * stage4.getState();
            
            T2 integratorInput;
            if (passBandGain)
                integratorInput = (x * (1.0 + feedbackFactor) - feedbackFactor * feedbackSum) * gainFactor;
            else
                integratorInput = (x - feedbackFactor * feedbackSum) * gainFactor;
            
            if (distortionFactor)
                integratorInput = std::tanh(integratorInput * *distortionFactor);
            
            auto y1 = stage1(integratorInput);
            auto y2 = stage2(y1);
            auto y3 = stage3(y2);
            auto y4 = stage4(y3);
            
            return xpanderMode ? computeXpander(integratorInput, y1, y2, y3, y4) : y4;
        }
        
    private:
        //! Compute Oberheim Xpander output
        T1 computeXpander(const T2& integratorInput, const T2& y1, const T2& y2, const T2& y3, const T2& y4)
        {
            switch (*xpanderMode)
            {
                case XpanderMode::SECONDORDERLOWPASS:
                    return y2;
                    break;
                    
                case XpanderMode::SECONDORDERHIGHPASS:
                    return -2 * y1 + y2;
                    break;
                    
                case XpanderMode::SECONDORDERBANDPASS:
                    return 2 * y1 + -2 * y2;
                    break;
                    
                case XpanderMode::FOURTHORDERHIGHPASS:
                    return integratorInput + -4 * y1 + 6 * y2 + -4 * y3 + y4;
                    break;
                    
                case XpanderMode::FOURTHORDERBANDPASS:
                    return 4 * y2 + -8 * y3 + 4 * y4;
                    break;
            }
        }
        
    public:
        //! Optional Oberheim Xpander mode
        std::experimental::optional<XpanderMode> xpanderMode;
        
    private:
        //! Low-pass filter stage 1
        AnalogOnePoleFilter<T1, T2> stage1;
        
        //! Low-pass filter stage 2
        AnalogOnePoleFilter<T1, T2> stage2;
        
        //! Low-pass filter stage 3
        AnalogOnePoleFilter<T1, T2> stage3;
        
        //! Low-pass filter stage 4
        AnalogOnePoleFilter<T1, T2> stage4;
        
        //! Feedback factor
        T2 feedbackFactor = 1;
        
        //! Feedback factor for first low-pass filter stage
        T2 feedbackFactorLP1 = 0;
        
        //! Feedback factor for second low-pass filter stage
        T2 feedbackFactorLP2 = 0;
        
        //! Feedback factor for third low-pass filter stage
        T2 feedbackFactorLP3 = 0;
        
        //! Feedback factor for fourth low-pass filter stage
        T2 feedbackFactorLP4 = 0;
        
        //! Filter gain factor with resolved zero delay feedback
        T2 gainFactor = 0;
        
        //! Optinal distortion factor for non-linear processing
        std::experimental::optional<T2> distortionFactor;
        
        //! Pass-band gain compensation, used in ARP filter models
        bool passBandGain = false;

    };
}

#endif /* AnalogLadderFilter_hpp */
