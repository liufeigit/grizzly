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

#ifndef GRIZZLY_SEGMENT_ENVELOPE_HPP
#define GRIZZLY_SEGMENT_ENVELOPE_HPP

#include <cassert>
#include <cstddef>
#include <dsperados/math/utility.hpp>
#include <dsperados/math/ease.hpp>
#include <dsperados/math/interpolation.hpp>
#include <experimental/optional>
#include <functional>
#include <initializer_list>
#include <numeric>
#include <unit/time.hpp>
#include <utility>
#include <vector>

namespace dsp
{
    //! Segment Envelope
    /*! Envelope generator using segments.
        A segment represents a single path over time. */
    template <class Value, class Time = float>
    class SegmentEnvelope
    {
    public:
        //! Segment class which describes the individual paths of the envelope
        class Segment
        {
        public:
            //! Construct a segment
            /*! @param destination The destination value, starting from the pervious destination or zero at envelope start
                @param duration: The duration in seconds to get to the destination.
                @param ease An: easing function to alther the shape of the segment */
            Segment(Value destination, unit::second<Time> duration, std::function<double(double)> ease = nullptr) :
                destination(destination),
                duration(duration),
                ease(ease)
            {
                
            }
            
            //! Given a time within the segment and its starting value, compute an interpolated value
            Value interpolate(unit::second<Time> time, const Value& startValue)
            {
                return math::interpolateLinear(computeTimeProportion(time), startValue, destination);
            }
            
        public:
            //! Destination value
            Value destination = 0;
            
            //! Duration
            unit::second<Time> duration = 0;
            
            //! Ease function to shape the segment
            std::function<double(double)> ease = [](auto x){ return math::easeLinear(x); };
            
        private:
            //! Return the eased proportion along the time axis of this axis
            double computeTimeProportion(unit::second<Time> time)
            {
                assert(duration.value != 0); // This function should never be called with a duration of 0
                double x = time.value / static_cast<double>(duration.value);
                return ease ? ease(x) : x;
            }
        };
        
    public:
        //! Construct an envelope without any segments
        SegmentEnvelope() = default;
        
        //! Construct an envelope with segments
        SegmentEnvelope(std::initializer_list<Segment> segments) :
            segments{segments}
        {
            
        }
        
        //! Increment the time
        void increment(unit::second<Time> increment);
        
        //! Return the current envelope value
        Value read();
        
        //! Set the state of the envelope
        /*! Jump directly to a certain point, changing the current segment accordingly. */
        void setTime(unit::second<Time> to);
        
        //! Reset the envelope to its starting position
        void reset() { setTime(0); }
        
    // --- Segment insertion/removal --- //
        
        //! Add a segment
        template <typename... Args>
        void addSegment(Args&&... args)
        {
            segments.emplace_back(std::forward<Args&&>(args)...);
        }
        
        //! Insert a segment
        template <typename... Args>
        void insertSegment(std::size_t index, Args&&... args)
        {
            segments.insert(segments.begin() + index, std::forward<Args&&>(args)...);
            setTime(envelopeTime);
        }
        
        //! Erase a segment
        void eraseSegment(std::size_t index)
        {
            if (segments.empty() || index >= segments.size())
                return;
            
            segments.erase(segments.begin() + index);
            setTime(envelopeTime);
        }
        
    // --- Hold point manipulation --- //
        
        //! Set a hold point (and enable it on default)
        void setAndEnableHoldPoint(unit::second<Time> at)
        {
            hold = Hold{at, true};
        }
        
        //! Enable the holdpoint, if there is one
        void enableHold()
        {
            if (hold)
                hold->enabled = true;
        }
        
        //! Disable the holdpoint, if there is one
        /*! The hold time will be remembered for enable calls in the future */
        void disableHold()
        {
            if (hold)
                hold->enabled = false;
        }
        
        //! Remove hold point
        void removeHoldPoint()
        {
            hold = std::experimental::nullopt;
        }
        
    // --- Access to the segments --- //
        
        //! Get a segment
        auto& operator[](size_t index) { return segments[index]; }
        
        //! Get a segment, const
        const auto& operator[](size_t index) const { return segments[index]; }
        
    // --- Utility construction functions --- //
        
        //! Create an attack, sustain, release envelope
        static SegmentEnvelope ar(unit::second<Time> attack, unit::second<Time> release, bool hold = true);
        
        //! Create an attack, decay, sustain, release envelope
        static SegmentEnvelope adsr(unit::second<Time> attack, unit::second<Time> decay, Value sustain, unit::second<Time> release);
        
    private:
        //! Hold point
        /*! Containts a time point from which the envelope stops incrementing and a boolian to indicate whether the hold is enabled or not. */
        struct Hold
        {
            unit::second<Time> timePoint = 0;
            bool enabled = false;
        };
        
    private:
        //! All the segments in the envelope (in sequential order)
        std::vector<Segment> segments;
        
        //! The segment index
        size_t index = 0;
        
        //! The current time in the current segment
        unit::second<Time> segmentTime = 0;
        
        //! The current time in the entire envelope
        unit::second<Time> envelopeTime = 0;
        
        //! Optional hold point
        /*! The envelope will remain at this point until the hold point is disable */
        std::experimental::optional<Hold> hold = std::experimental::nullopt;
    };
    
    template <class Value, class Time>
    void SegmentEnvelope<Value, Time>::increment(unit::second<Time> increment)
    {
        if (segments.empty() || index >= segments.size())
            return;
        
        segmentTime += increment;
        envelopeTime += increment;
        
        if (hold && hold->enabled && envelopeTime >= hold->timePoint)
        {
            segmentTime -= envelopeTime - hold->timePoint;
            envelopeTime = hold->timePoint;
        }
        
        while (index < segments.size() && segmentTime >= segments[index].duration)
        {
            segmentTime -= segments[index].duration;
            ++index;
        }
    }
    
    template <class Value, class Time>
    Value SegmentEnvelope<Value, Time>::read()
    {
        if (segments.empty())
            return 0;
        else if (index >= segments.size())
            return segments.back().destination;
        
        return segments[index].interpolate(segmentTime, index == 0 ? Value(0) : segments[index - 1].destination);
    }
    
    template <class Value, class Time>
    void SegmentEnvelope<Value, Time>::setTime(unit::second<Time> to)
    {
        if (segments.empty() || to <= 0)
        {
            envelopeTime = 0;
            segmentTime = 0;
            index = 0;
            return;
        }
        
        // accumulate all segments durations
        auto envelopeDuration = std::accumulate(segments.begin(), segments.end(), unit::second<Time>(0) , [](const auto& acc, const auto& segment) { return acc + segment.duration; } );
        
        if (to >= envelopeDuration)
        {
            envelopeTime = envelopeDuration;
            segmentTime = segments.back().duration;
            index = segments.size();
            return;
        }
        
        envelopeTime = to;
        
        unit::second<Time> partialTime = 0;
        for (auto i = 0; i < segments.size(); ++i)
        {
            if (partialTime + segments[i].duration < envelopeTime)
            {
                partialTime += segments[i].duration;
                continue;
            }
            
            index = i;
            segmentTime = envelopeTime - partialTime;
            break;
        }
    }
    
    template <class Value, class Time>
    SegmentEnvelope<Value, Time> SegmentEnvelope<Value, Time>::ar(unit::second<Time> attack, unit::second<Time> release, bool hold)
    {
        SegmentEnvelope<Value, Time> env = {{1, attack}, {0, release}};
        if (hold)
            env.setAndEnableHoldPoint(attack);
        return env;
    }
    
    template <class Value, class Time>
    SegmentEnvelope<Value, Time> SegmentEnvelope<Value, Time>::adsr(unit::second<Time> attack, unit::second<Time> decay, Value sustain, unit::second<Time> release)
    {
        SegmentEnvelope<Value, Time> env = {{1, attack}, {sustain, decay}, {0, release}};
        env.setAndEnableHoldPoint(attack + decay);
        return env;
    }
}


#endif
