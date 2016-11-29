
#ifndef GRIZZLY_SEGMENTENVELOPE
#define GRIZZLY_SEGMENTENVELOPE

#include <dsperados/math/utility.hpp>
#include <initializer_list>
#include <dsperados/math/ease.hpp>
#include <dsperados/math/interpolation.hpp>
#include <functional>
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
        
    public:
        SegmentEnvelope() = default;
        
        SegmentEnvelope(std::initializer_list<Segment> segments) :
        segments{segments}
        {
        }
        
        
        
        //! Increment the time
        void increment(const Time& increment)
        {
            
            
            time += increment;
            
            while (time >= segments[index].duration && index < segments.size())
            {
                time -= segments[index].duration;
                ++index;
                
                if (loopMode && index == loopPoints.second)
                    index = loopPoints.first;
            }
            
        }
        
        //! Return the current envelope value
        Value read()
        {
            
            
            
            if (index >= segments.size())
                return segments.back().destination;
            
            auto currentSegment = segments[index];
            
            auto normalizedOutput = currentSegment.ease(time / static_cast<double>(currentSegment.duration));
            
            return math::interpolateLinear(normalizedOutput, index == 0 ? Value(0) : segments[index - 1].destination, currentSegment.destination);
        }
        
        
        
    private:
        //! Vector for Segments
        std::vector<Segment> segments;
        
        //! The segment index
        size_t index = 0;
        
        unit::second<Time> time = 0;
        
        std::pair<size_t, size_t> loopPoints = {1, 2};
        
        bool loopMode = true;
    };
}


#endif
