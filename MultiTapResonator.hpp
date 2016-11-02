//
//  MultiResonator.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 31/10/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//


#ifndef GRIZZLY_MUTLI_TAP_RESONATOR_HPP
#define GRIZZLY_MUTLI_TAP_RESONATOR_HPP

#include <cstddef>
#include <functional>
#include <numeric>
#include <vector> 

#include "Delay.hpp"
#include "Filter.hpp"


namespace bear::dsp
{
    //! Multi tap resonator
    template <class T>
    class MultiTapResonator : public dsp::Filter<T>
    {
    public:
        //! Multi Tap delay tap info, the postDelay function can be changed by the user to add functionalities
        class Stage
        {
        public:
            //! processing of the delay tap
            /*! if the postDelay function has not been declared, the delay and the feedback will simply be applied */
            T processStage (const dsp::Delay<T>& delay) const
            {
                const auto d = delay.read(delayTime);
                return feedback * (postDelay ? postDelay(d) : d);
            }
            
        public:
            //! PostDelay function
            std::function<T(const T&)> postDelay;
            
            double delayTime;
            
            //! Feedback, 0. to 1.
            /*!  If there are multiple delay lines, it is recommended to set the feedback to 1 / number of stages. */
            double feedback;
        };
        
    public:
        //! Create a multi tap resonator
        /*! Initialize the delay buffer with an initial maximum length and create a number of delay lines,
            it is posible to initialize the gain compensation*/
        MultiTapResonator (const std::size_t maxDelay, const std::size_t numberOfStages,const bool gainCompensation = false) :
            maxDelay(maxDelay),
            delay(maxDelay),
            stages(numberOfStages),
            gainCompensation(gainCompensation)
        {
            
        }
        
        //! Process function, take an input, execute all delay taps and write the output in the delay line.
        T process (const T& x) final override
        {
            auto y = std::accumulate(stages.begin(), stages.end(), T(), [&](const T& acc, auto& stage)
            {
                return acc + stage.processStage(delay);
            });
            
//            if (gainCompensation)
////                y /= stages.size();
            
            y += x;
            delay.write(y);
            
            return y;
        }
        
        //! Gain Compensation
        /*! If the gain compensation is activated the output of the taps will be scaled by the reciprocal 
            of the number of taps */
        void setGainCompensation (const bool gainCompensation)
        {
            this->gainCompensation = gainCompensation;
        }
        
        //! Resize the delay buffer
        void resize (const std::size_t maxDelay)
        {
            this->maxDelay = maxDelay;
            delay.resize(maxDelay);
        }
        
        //! Get the maximum size of the delay buffer
        const std::size_t getMaxDelaySize () const
        {
            return maxDelay;
        }
        
        //! Overide operator[] to allow easy acces to the delay lines
        Stage& operator[](const std::size_t index)
        {
            return stages[index];
        }
        
        // Add iterative functionalities to allow easy acces to the delay lines
        auto begin() { return stages.begin(); }
        auto end() { return stages.end(); }
        
        
    private:
        //! Delay Lines
        std::vector<Stage> stages;
        
        std::size_t maxDelay;
        
        //! The single delay buffer that will be used for all stages
        dsp::Delay<T> delay;
        
        //! Gain Compensation
        bool gainCompensation;
    };
}

#endif /* GRIZZLY_MUTLI_TAP_RESONATOR_HPP */
