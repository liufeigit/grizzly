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

namespace bear::dsp
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
        
        //! Process, return the envelope output
        T operator()()
        {
            
            //if (index == 2) goToSegment(1);
            
            // End of the envelope, return 0
            if (index >= segments.size())
                return 0;
            
            // Iterate over the hold points
            for (auto iterator = holdPoints.begin(); iterator != holdPoints.end(); ++iterator)
            {
                auto time = iterator->first;
                auto& value = iterator->second;
                
                // hold if the current time > hold time and set the hold value to false to trigger this action just once
                if (timeInEnvelope >= time && value)
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
        
        //! Go to the next segment, up the index and recompute increment
        void goToNextSegment()
        {
            index++;
            computeIncrement(index);
        }
        
        void goToSegment(size_t index)
        {
            this->index = index;
            computeIncrement(index);
        }
        
    public:
        //! Boolian for (un)holding
        bool onHold = false;
        
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
    };
}


#endif
