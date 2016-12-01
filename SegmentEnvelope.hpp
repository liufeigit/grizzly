
#ifndef GRIZZLY_SEGMENTENVELOPE
#define GRIZZLY_SEGMENTENVELOPE


#include <dsperados/math/utility.hpp>
#include <initializer_list>
#include <dsperados/math/ease.hpp>
#include <dsperados/math/interpolation.hpp>
#include <experimental/optional>
#include <functional>
#include <numeric>
#include <unit/time.hpp>
#include <utility>
#include <vector>

namespace dsp
{
    //! Segment Envelope
    /*! Envelope generator using segments.
     A segment represents a single path over time. */
    template <class Value, class Time = double>
    class SegmentEnvelope
    {
    public:
        //! Segment class which describes the individual paths of the envelope
        class Segment
        {
        public:
            //! Construct a segment
            /*! @param destination The destination value, starting from the pervious destination or zero at envelope start
             @param duration The duration in seconds to get to the destination. */
            Segment(Value destination, Time duration) :
            destination(destination),
            duration(duration)
            {
                
            }
            
            //! Construct a segment
            /*! @param destination The destination value, starting from the pervious destination or zero at envelope start
             @param duration The duration in seconds to get to the destination.
             @param ease An easing function to alther the shape of the segment */
            Segment(Value destination, Time duration, std::function<double(double)> ease) :
            destination(destination),
            duration(duration),
            ease(ease)
            {
            }
            
            //! Destination value
            Value destination = 0;
            
            //! Duration
            unit::second<Time> duration = 0;
            
            //! Ease function to shape the segment
            std::function<double(double)> ease = math::easeLinear<double>;
        };
        
        
    public:
        //! Construct an envelope without any segments
        SegmentEnvelope() = default;
        
        //! Construct an envelope with segments
        SegmentEnvelope(std::initializer_list<Segment> segments) :
        segments{segments}
        {
        }
        
        //! Set the state of the envelope
        /*! Jump directly to a certain point, changing the current segment accordingly. */
        void setState(unit::second<Time> to)
        {
            auto envelopeDuration = std::accumulate(segments.begin(), segments.end(), unit::second<Time>(0) , [](const auto& acc, const auto& segment) { return acc + segment.duration; } );
            
            if (to >= envelopeDuration)
            {
                envelopeTime = envelopeDuration;
                segmentTime = segments.back().duration;
                index = segments.size();
                
                if (hold)
                    hold->enabled = false;
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
        
        //! Set a hold point
        void setAndEnableHoldPoint(unit::second<Time> at)
        {
            hold = Hold{at, true};
        }
        
        //! Disable the holdpoint, if there is one
        void disableHold()
        {
            if (hold)
                hold->enabled = false;
        }
        
        //! Enable the holdpoint, if there is one
        void enableHold()
        {
            if (hold)
                hold->enabled = true;
        }
        
        //! Remove hold point
        void removeHoldPoint()
        {
            hold = std::experimental::nullopt;
        }
        
    public:
        //! Vector for Segments
        std::vector<Segment> segments;
        
        
    private:
        //! Hold structure
        /*! Containts a time point from which the envelope stops incrementing and a boolian to indicate whether the hold is enabled or not. */
        struct Hold
        {
            unit::second<Time> timePoint = 0;
            bool enabled = false;
        };
        
        
    private:
        //! The segment index
        size_t index = 0;
        
        //! The current time in the current segment
        unit::second<Time> segmentTime = 0;
        
        //! The current time in the whole envelope
        unit::second<Time> envelopeTime = 0;
        
        //! Optinal hold point
        std::experimental::optional<Hold> hold = std::experimental::nullopt;
    };
}


#endif
