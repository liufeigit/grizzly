//
//  MultiResonator.hpp
//  octoSandbox1
//
//  Created by Vincent Pruijn on 31/10/16.
//  Copyright © 2016 Dsperados. All rights reserved.
//


#ifndef GRIZZLY_MUTLI_TAP_RESONATOR_HPP
#define GRIZZLY_MUTLI_TAP_RESONATOR_HPP

#include <cmath>
#include <cstddef>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <vector> 

#include "Delay.hpp"

namespace dsp
{
    //! Multi tap resonator
    template <class T>
    class MultiTapResonator
    {
    public:
        //! Delay tap info
        /*! the postDelay function can be changed by the user to add functionalities */
        class Stage
        {
        public:
            //! Process a single tap in the resonator
            T processStage (const dsp::Delay<T>& delay) const
            {
                const auto d = delay.read(delayTime);
                return feedback * (postDelay ? postDelay(d) : d);
            }
            
        public:
            //! The delay time for this stage in the resonator
            double delayTime;
            
            //! Feedback, 0. to 1.
            /*! If there are multiple delay lines, it is recommended to set the feedback to (1 / number of stages). */
            double feedback = 0;
            
            //! PostDelay function
            std::function<T(const T&)> postDelay;
        };
        
    public:
        //! Create a multi tap resonator
        /*! Initialize the delay buffer with an initial maximum length and create a number of delay lines,
            it is posible to initialize the gain compensation*/
        MultiTapResonator(std::size_t maxDelay, std::size_t numberOfStages) :
            stages(numberOfStages),
            delay(maxDelay)
        {
            
        }
        
        MultiTapResonator(std::initializer_list<Stage> stages) :
            stages{stages},
            delay(0)
        {
            auto it = std::max_element(stages.begin(), stages.end(), [](const auto& lhs, const auto& rhs){ return lhs.delayTime < rhs.delayTime; });
            if (it != stages.end())
                delay.resize(std::ceil(it->delayTime));
        }
        
        //! Take an input, execute all delay taps and write the output in the delay line.
        /*! @param x: The input to the resonator
            @param compensateGain: Should all taps be divided by the number of taps, that they add up to 1? */
        void write(const T& x, bool compensateGain = false)
        {
            auto y = std::accumulate(stages.begin(), stages.end(), T(), [&](const T& acc, auto& stage) { return acc + stage.processStage(delay); });
            
            if (compensateGain)
                y /= static_cast<double>(stages.size());
            
            delay.write(x + y);
        }
        
        //! Read the most recently computed value
        T read() const { return delay.read(0); }
        
        //! Resize the delay buffer
        void resize(std::size_t maxDelay)
        {
            delay.resize(maxDelay);
        }
        
        //! Get the maximum size of the delay buffer
        std::size_t getMaximumDelayTime() const
        {
            return delay.getMaximumDelayTime();
        }
        
        //! Overide operator[] to allow easy acces to the delay lines
        Stage& operator[](const std::size_t index)
        {
            return stages[index];
        }
        
        //! Return the size of the resonator (the number of stages)
        std::size_t size() const { return stages.size(); }
        
        //! Is the resonator empty of stages?
        bool empty() const { return stages.empty(); }
        
        // Add iterative functionalities to allow easy acces to the delay lines
        auto begin() { return stages.begin(); }
        auto begin() const { return stages.begin(); }
        auto end() { return stages.end(); }
        auto end() const { return stages.end(); }
        
    private:
        //! Delay Lines
        std::vector<Stage> stages;
        
        //! The single delay buffer that will be used for all stages
        dsp::Delay<T> delay;
    };
}

#endif /* GRIZZLY_MUTLI_TAP_RESONATOR_HPP */
