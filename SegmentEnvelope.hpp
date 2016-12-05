
#ifndef GRIZZLY_SEGMENTENVELOPE
#define GRIZZLY_SEGMENTENVELOPE


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
        
        //! Reset the envelope
        void reset()
        {
            setState(0);
        }
        
        //! Increment the time
        void increment(const Time& increment)
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
        
        //! Return the current envelope value
        Value read()
        {
            if (segments.empty())
                return 0;
            else if (index >= segments.size())
                return segments.back().destination;
            
            auto currentSegment = segments[index];
            
            auto normalizedOutput = currentSegment.ease(segmentTime / static_cast<double>(currentSegment.duration));
            
            return math::interpolateLinear(normalizedOutput, index == 0 ? Value(0) : segments[index - 1].destination, currentSegment.destination);
        }
        
        //! Add a segment
        void addSegment(Segment segment)
        {
            segments.emplace_back(segment);
        }
        
        //! Insert a segment
        void insertSegment(size_t index, Segment segment)
        {
            segments.insert(segments.begin() + index, segment);
            setState(envelopeTime);
        }
        
        //! Erase a segment
        void eraseSegment(size_t index)
        {
            if (segments.empty() || index >= segments.size())
                return;
            
            segments.erase(segments.begin() + index);
            setState(envelopeTime);
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
        
        //! Get a segment
        Segment& operator[](size_t index) { return segments[index]; }
        
        //! Get a segment, const
        const Segment& operator[](size_t index) const { return segments[index]; }
        
        
    private:
        //! Hold point
        /*! Containts a time point from which the envelope stops incrementing and a boolian to indicate whether the hold is enabled or not. */
        struct Hold
        {
            unit::second<Time> timePoint = 0;
            bool enabled = false;
        };
        
        
    private:
        //! Vector for Segments
        std::vector<Segment> segments;
        
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
