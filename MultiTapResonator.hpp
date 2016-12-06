/*
 
 This file is a part of Grizzly, a modern C++ library for digital signal
 processing. See https://github.com/dsperados/grizzly for more information.
 
 Copyright (C) 2016 Dsperados <info@dsperados.com>
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 
 --------------------------------------------------------------------
 
 If you would like to use Grizzly for commercial or closed-source
 purposes, please contact us for a commercial license.
 
 */

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
