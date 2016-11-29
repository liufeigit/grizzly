
#ifndef GRIZZLY_SEGMENTENVELOPE
#define GRIZZLY_SEGMENTENVELOPE


#include <dsperados/math/utility.hpp>
#include <initializer_list>
#include <dsperados/math/ease.hpp>
#include <dsperados/math/interpolation.hpp>
#include <experimental/optional>
#include <functional>
#include <map>
#include <numeric>
#include <unit/time.hpp>
#include <utility>
#include <vector>

#include <iostream>

namespace dsp
{
    //! Segment Envelope
    /*! Envelope generator using segments.
     A segment represents a single path over time. */
    template <class Value, class Time = double>
    class SegmentEnvelope
    {
    public:
        class Segment
        {
        public:
            Segment(Value destination, Time duration) :
            destination(destination),
            duration(duration)
            {
                
            }
            
            Segment(Value destination, Time duration, std::function<double(double)> ease) :
            destination(destination),
            duration(duration),
            ease(ease)
            {
            }
            
            Value destination = 0;
            unit::second<Time> duration = 0;
            std::function<double(double)> ease = math::easeLinear<double>;
        };
        
        struct Hold
        {
            unit::second<Time> timePoint = 0;
            bool enabled = false;
        };
        
    public:
        SegmentEnvelope() = default;
        
        SegmentEnvelope(std::initializer_list<Segment> segments) :
        segments{segments}
        {
        }
        
        void setState(unit::second<Time> to)
        {
            auto envelopeDuration = std::accumulate(segments.begin(), segments.end(), unit::second<Time>(0) , [](const unit::second<Time>& acc, const Segment& segment) { return acc + segment.duration; } );
            
            if (to >= envelopeDuration)
            {
                envelopeTime = envelopeDuration;
                segmentTime = segments.back().duration;
                index = segments.size();
            }
            else
            {
                envelopeTime = to;
                
                unit::second<Time> parialTime = 0;
                for (auto i = 0; i < segments.size(); ++i)
                {
                    if (parialTime + segments[i].duration < envelopeTime)
                    {
                        parialTime += segments[i].duration;
                        continue;
                    }
                    
                    index = i;
                    segmentTime = envelopeTime - parialTime;
                    break;
                }
            }
        }
        
        void unHold()
        {
            if (hold)
                hold->enabled = false;
        }
        
        //! Increment the time
        void increment(const Time& increment)
        {
            segmentTime += increment;
            envelopeTime += increment;
            
            if (hold && hold->enabled && envelopeTime >= hold->timePoint)
            {
                segmentTime -= envelopeTime - hold->timePoint;
                envelopeTime = hold->timePoint;
            }
            
            while (segmentTime >= segments[index].duration && index < segments.size())
            {
                segmentTime -= segments[index].duration;
                ++index;
            }
        }
        
        //! Return the current envelope value
        Value read()
        {
            if (index >= segments.size())
                return segments.back().destination;
            
            auto currentSegment = segments[index];
            
            auto normalizedOutput = currentSegment.ease(segmentTime / static_cast<double>(currentSegment.duration));
            
            return math::interpolateLinear(normalizedOutput, index == 0 ? Value(0) : segments[index - 1].destination, currentSegment.destination);
        }
        
        //! Optinal hold point
        std::experimental::optional<Hold> hold = std::experimental::nullopt;
        
    private:
        //! Vector for Segments
        std::vector<Segment> segments;
        
        //! The segment index
        size_t index = 0;
        
        
        unit::second<Time> segmentTime = 0;
        unit::second<Time> envelopeTime = 0;
        
    };
}


#endif
