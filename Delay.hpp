//
//  Delay.hpp
//  libbear
//
//  Created by Stijn on 02/05/2015.
//  Copyright (c) 2015 FrisHertz. All rights reserved.
//

#ifndef BEAR_DSP_DELAY_HPP
#define BEAR_DSP_DELAY_HPP

#include <boost/circular_buffer.hpp>
#include <dsperados/math/interpolation.hpp>

namespace dsp
{
    //! A simple sample delay object
    /*! Delay based on a circular buffer, capable of interpolation */
    template <class T>
    class Delay
    {
    public:
        //! Construct with an empty delay line
        Delay() = default;
        
        //! Construct by feeding the maximum delay size
        Delay(std::size_t maxDelay)
        {
            resize(maxDelay);
        }
        
        //! Push a sample in the delay line
        void write(const T& x)
        {
            data.push_front(x);
        }
        
        //! Read from the delay line
        template <class Index, class Interpolator = math::LinearInterpolation>
        T read (Index index, Interpolator interpolator = Interpolator()) const
        {
            return interpolate(data.begin(), data.end(), index, interpolator, math::ClampedAccess());
        }
        
        //! Set the maximum delay
        void resize(std::size_t maxDelay)
        {
            data.resize(maxDelay + 1);
        }
        
        //! Return the maximum number of delay samples
        std::size_t getMaxDelay() const { return data.size() - 1; }
        
        //! Begin for range for-loops
        inline auto begin() { return data.begin(); }
        
        //! Begin for range for-loops
        inline auto begin() const { return data.begin(); }
        
        //! Begin for range for-loops
        inline auto rbegin() { return data.rbegin(); }
        
        //! Begin for range for-loops
        inline auto rbegin() const { return data.rbegin(); }
        
        //! End for range for-loops
        inline auto end() { return data.end(); }
        
        //! End for range for-loops
        inline auto end() const { return data.end(); }
        
        //! End for range for-loops
        inline auto rend() { return data.rend(); }
        
        //! End for range for-loops
        inline auto rend() const { return data.rend(); }
        
    private:
        //! The data in the delay line
        boost::circular_buffer<T> data;
    };
}

#endif
