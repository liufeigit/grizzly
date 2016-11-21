//
//  APAudioEnvelope.h
//  MTGSC
//
//  Created by Danny van Swieten on 20-05-14.
//
//

#ifndef GRIZZLY_ENVELOPE
#define GRIZZLY_ENVELOPE

#include <map>
#include <vector>

#include "Ramp.hpp"

namespace dsp
{
    //! Envelope
    /*! Envelope generator build-up from segments. Each segment describes a slope over time. */
    template <class T>
    class Envelope
    {
    public:
        //! Constructor
        /*! @param sampleRate The sampleRate
            @param segments A vector of tuples containing the segment time and slope function */
        Envelope(T sampleRate, std::vector<std::tuple<T, std::function<T(T)>>> segments) : sampleRate(sampleRate)
        {
            // Add segments
            for (auto& segment: segments)
                this->segments.emplace_back(std::get<0>(segment), std::get<1>(segment));
            
            // Compute increment for the first segment
            computeIncrement(0);
            
            // On segment end, up the index and recompute increment
            ramp.end = [&]{ goToNextSegment(); };
        }
        
        //! Set the duration of a segment
        void setSegmentDuration(size_t index, T duration)
        {
            segments[index].duration = duration;
            computeIncrement(index);
        }
        
        //! Set the behaviour of a segment
        void setSegmentBehaviour(size_t index, std::function<T(T)> behaviour)
        {
            segments[index].behaviour = behaviour;
        }
        
        //! Set a hold point
        void addHoldPoint(T time)
        {
            holdPoints[time] = true;
        }
        
        //! Remove a hold point
        void removeHoldPoint(T time)
        {
            holdPoints.erase(time);
        }
        
        //! Go to the next segment, up the index and recompute increment
        void goToNextSegment()
        {
            index++;
            computeIncrement(index);
        }
        
        //! Go to a segment, up the index and recompute increment
        void goToSegment(size_t index)
        {
            this->index = index;
            computeIncrement(index);
        }
        
        //! Set loop indecies
        void setLoop(size_t fromIndex, size_t toIndex)
        {
            loopIndecies[0] = fromIndex;
            loopIndecies[1] = toIndex;
        }
        
        //! Process, return the envelope output
        T operator()()
        {
            // Loop
            if (loop && index == loopIndecies[1])
            {
                goToSegment(loopIndecies[0]);
                
                // Reset time in envelope
                timeInEnvelope = 0;
                
                //add durations of previous segments before the loop start index
                for (auto segment = 0; segment < loopIndecies[0]; ++segment)
                    timeInEnvelope += segments[segment].duration;
                
                // Reset hold-points to true within the loop indecies,
                for (auto iterator = holdPoints.begin(); iterator != holdPoints.end(); ++iterator)
                {
                    auto holdTime = iterator->first;
                    auto timeInEnvelopeAtLoopEnd = timeInEnvelope + segments[loopIndecies[1]].duration;
                    
                    if (holdTime >= timeInEnvelope && holdTime < timeInEnvelopeAtLoopEnd)
                        iterator->second = true;
                }
            }
            
            // End of the envelope, reset hold-points to true and return 0
            if (index >= segments.size())
            {
                for (auto iterator = holdPoints.begin(); iterator != holdPoints.end(); ++iterator)
                    iterator->second = true;
                
                return 0;
            }
            
            // Iterate over the hold points
            for (auto iterator = holdPoints.begin(); iterator != holdPoints.end(); ++iterator)
            {
                auto holdTime = iterator->first;
                auto& value = iterator->second;
                
                // hold if the current time > hold time and set the hold value to false to trigger this action just once
                if (timeInEnvelope >= holdTime && value)
                {
                    value = false;
                    onHold = true;
                }
            }

            // If onHold, don't increment the ramp and timeInEnvelope
            if (onHold)
                return segments[index].behaviour(ramp(0));
            
            // Increment time
            timeInEnvelope += 1 / sampleRate;
            
            return segments[index].behaviour(ramp(increment));
        }
        
    public:
        //! Boolian for (un)holding
        bool onHold = false;
        
        //! Loop on/off
        bool loop = false;
        
    private:
        //! Segment class to define the behaviour and time for each segment
        /*! The behaviour is a functions which describes the slope of the segment. 
            See Dsperados math::easing for some pre-built functions (e.g. linear and exponential). */
        class Segment
        {
        public:
            Segment(T duration, std::function<T(T)> behaviour) :
            duration(duration),
            behaviour(behaviour)
            {
            }
            
            T duration = 0;
            std::function<T(T)> behaviour = [](T x){ return x; };
        };
        
    private:
        //! Compute the increment for the a segment
        void computeIncrement(size_t index)
        {
            increment = 1.0 / (this->segments[index].duration * this->sampleRate);
        }

    private:
        //! Vector for Segments
        std::vector<Segment> segments;
        
        //! The sample rate
        T sampleRate = 44100;
        
        //! The segment index
        size_t index = 0;
        
        //! Ramp for driving the envelope behaviour
        dsp::Ramp<T> ramp;
        
        //! Increment for the Ramp
        T increment = 0;
        
        //! Time in envelope to keep track of hold points
        T timeInEnvelope = 0;
        
        //! The hold points
        std::map<T, bool> holdPoints;
        
        //! Loop indecies
        size_t loopIndecies[2] = {0, 0};
    };
}


#endif
